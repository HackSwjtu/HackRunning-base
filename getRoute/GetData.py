# -*- coding: utf-8 -*-
import json
import uuid
import time
import base64
import random
import requests
from math import radians, cos, sin, asin, sqrt

Success = 10000
MaxDistance = 50

def Base64Encode(username, pwd):
    List = [username, pwd]
    sign = ':'
    strr = sign.join(List)
    return "Basic " + str(base64.b64encode(strr))

def selectRoute(count = 1):
    return int(random.uniform(0, count - 1))


class VirtualRunSession(object):

    'login dataUpload logout'

    def __init__(self, username, password):

        super(VirtualRunSession, self).__init__()

        self.uid = 79473
        self.unid = 901
        self.roomIds = []
        self.totalTime = None
        self.totalDis = None
        self.startTime = None
        self.stopTime = None
        self.selDistance = None
        self.allLocJson = None
        self.fivePointJson = None
        self.username = username
        self.password = password
        self.url = 'http://gxapp.iydsj.com/api/'
        self.deviceId = str(uuid.uuid1()).upper()
        self.headers = {
            "Host": "gxapp.iydsj.com",
            "Accept": "*/*",
            "Authorization": Base64Encode(username, password),
            "Proxy-Connection": "keep-alive",
            "osType": "appVersion",
            "Accept-Encoding": "gzip, deflate",
            "Accept-Language": "zh-Hans-CN;q=1",
            "Content-Type": "application/x-www-form-urlencoded",
            "DeviceId": self.deviceId,
            "CustomDeviceId": self.deviceId + "_iOS_sportsWorld_campus",
            "User-Agent": "SWCampus/1.2.0 (iPhone; iOS 9.3.4; Scale/3.00)",
            "appVersion":"1.2.0"
        }


    def getRoomId(self):
        url = self.url + ("v3/get/aboutrunning/list/%s/%s/3"
                          % (str(self.uid), str(self.unid)))
        headers = self.headers
        headers["osType"] = "1"
        headers["uid"] = str(self.uid)

        sess = requests.session()
        request = sess.get(url, headers = headers)
        resp = request.content
        resp = json.loads(resp)
        if resp["error"] != Success:
            self.logout()
            raise Exception("Get room id error")
        self.roomIds = [item["roomId"] for item in resp["data"]]
        print "Get room id success"


    def getRoute(self):
        index = selectRoute(len(self.roomIds))
        print "use %s data" % index
        roomId = str(self.roomIds[index])
        url = self.url + ("v3/get/%s/history/finished/record" % roomId)
        headers = self.headers
        headers["osType"] = "1"
        headers["uid"] = str(self.uid)

        sess = requests.session()
        request = sess.get(url, headers = headers)
        resp = request.content
        resp = json.loads(resp)
        if resp["error"] != Success:
            self.logout()
            raise Exception("Get route error")
        print "Get route success"

        self.allLocJson = resp["data"]["roomersModelList"][0]["points"]

        self.writeToFile(self.allLocJson, "2016-10-24-route.data")

        tmp = json.loads(self.allLocJson)
        allLocJsons = json.loads(tmp["allLocJson"])
        self.totalTime = allLocJsons[-1]["totalTime"]
        self.totalDis = float(allLocJsons[-1]["totalDis"]) / 1000
        startTime = str(allLocJsons[0]["gainTime"])
        stopTime = str(allLocJsons[-1]["gainTime"])
        timeFormat = "%Y-%m-%d %H:%M:%S"
        self.startTime = long(time.mktime(time.strptime(startTime, timeFormat)) * 1000)
        self.stopTime = long(time.mktime(time.strptime(stopTime, timeFormat)) * 1000)
        self.selDistance = int(resp["data"]["roomInfoModel"]["distance"])
        speed = (self.totalTime/60) / self.totalDis

        otherJson = {"totalTime":self.totalTime, "totalDis":self.totalDis,
                     "startTime":self.startTime, "stopTime":self.stopTime,
                     "selDistance":self.selDistance, "speed":speed}
        self.writeToFile(json.dumps(otherJson), "2016-10-24-otherJson.data")


    def getFivePointJson(self):

        def getDis(lon1, lat1, lon2, lat2):
            """
            Calculate the great circle distance between two points
            on the earth (specified in decimal degrees)
            """
            lon1, lat1, lon2, lat2 = map(radians, [lon1, lat1, lon2, lat2])
            dlon = lon2 - lon1
            dlat = lat2 - lat1
            a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
            c = 2 * asin(sqrt(a))
            r = 6378.137
            return c * r * 1000

        with open("fivePoint.data", "r") as f:
            tmp = f.readlines()
        refPoints = []
        for item in tmp:
            item = item.split(" ")
            dic = {"lon":float(item[0]), "lat":float(item[1])}
            refPoints.append(dic)

        fivePoints = []
        tmp = json.loads(self.allLocJson)
        allLocJsons = json.loads(tmp["allLocJson"])
        for i in allLocJsons:
            for j in refPoints:
                if getDis(float(i["lng"]), float(i["lat"]),
                          j["lon"], j["lat"]) < MaxDistance \
                        and j not in fivePoints:
                    fivePoints.append(j)
                    break
            if len(fivePoints) >= 3:
                break
        while len(fivePoints) < 5:
            randPoint = random.choice(refPoints)
            if randPoint not in fivePoints:
                fivePoints.append(randPoint)

        id = int(random.uniform(10, 100))
        flag = allLocJsons[0]["flag"]
        for item in fivePoints:
            item["flag"] = flag
            item["isFinal"] = "false"
            item["isFixed"] = 0
            item["isPass"] = "false"
            item["id"] = id
            id += 1
        for i in range(3):
            fivePoints[i]["isPass"] = "true"
        fivePoints[1]["isFixed"] = 1

        fivePoints = json.dumps(fivePoints)

        fivePointJson = {"fivePointJson":str(fivePoints),
                         "useZip":"false"}

        self.writeToFile(json.dumps(fivePointJson), "2016-10-24-fivePoint.data")

        print "Get five point success."


    def writeToFile(self, text, filename):
        with open(filename, 'w') as f:
            f.writelines(text)


def main():

    username = ""
    password = ""

    session = VirtualRunSession(username, password)

    #try:
    session.getRoomId()
    session.getRoute()
    session.getFivePointJson()
    '''
        #session.writeToFile(session.allLocJson, "1023route.data")
    except Exception as e:
        #session.logout()
        print e'''


if __name__ == '__main__':
    main()

















