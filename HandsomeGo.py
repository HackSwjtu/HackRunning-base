#coding: utf-8
import requests,time

def main():

    fivePointJson=readFivePointJsonFile()
    allLocJson =readAllLocJsonFile()

    fivePointJson_dict = {"fivePointJson":fivePointJson, 'useZip':"false"}
    allLocJson_dict = {'allLocJson':allLocJson, 'useZip':"false"}

    url='http://gxapp.iydsj.com/api/v2/users/48405/running_records/add'
    headers={
        "appVersion": "1.2.0",
        "CustomDeviceId": "XXXXXXXXXXXXXXXXXXXXXXX",
        "DeviceId": "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
        "osType": "1",
        "source": "000049",
        "Authorization":"Basic XXXXXXXXXXXXXXXXX",
    }

    json={
        "allLocJson":str(allLocJson_dict),
        'complete':'True',
        "fivePointJson":str(fivePointJson_dict),
        "isUpload":"false",
        "totalTime":737 ,       #your time (s),
        "totalDis":"3.058769",#your distance XXX(km)
        "speed":"4.015",    #your speed
        "stopTime":1476619526802+737*1000   #time()*1000 + totaltime*1000,
        "startTime":1476619526802,      #about time()*1000
        "sportType":1,
        "selDistance":3,
        "unCompleteReason":0
    }
    s=requests.Session()
    r=s.post(url,headers=headers,json=json)
    print (r.content)

if __name__=='__main__':
    main()



