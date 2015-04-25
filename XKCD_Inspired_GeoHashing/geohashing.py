#XKCD inspired GeoHashing
 
import time, urllib, json, md5, math, webbrowser

def getHash():
    date = time.strftime("%Y-%m-%d")
    timeT = time.strftime("%H:%M:%S")
    to_md5 = date + timeT
    md5_text = md5.new(to_md5).hexdigest()
    print ("Date + time --md5-->  " + md5_text)
    return md5_text

def getLocalisation(a0, a1):
    a0 = str(int(a0, 16))
    a0 = a0[:6]
    a1 = str(int(a1, 16))
    a1 = a1[:6]
    print ("To decimal (and cut): " + str(a0) +" " + str(a1))

    ip = urllib.urlopen('http://canihazip.com/s').read()
    response = json.load(urllib.urlopen('http://ip-api.com/json/'+ip))
    lat = response['lat']
    lon = response['lon']
    lat = int(math.floor(lat))
    lon = int(math.floor(lon))
    finalLat = str(lat) +"."+ str(a0)
    finalLon = str(lon) +"."+ str(a1)
    return finalLat, finalLon

def open_browser(finalLat, finalLon):
    new = 2
    url = 'https://maps.google.com?saddr=Current+Location&daddr='+finalLat+','+finalLon
    webbrowser.open(url,new=new)

if __name__ == "__main__":
    print("########### Computing ############\n\n")
    md5_text = getHash()
    a0 = md5_text[:16]
    a1 = md5_text[16:]
    print ("            Splitted: " + a0 +" " + a1)
    (finalLat, finalLon) = getLocalisation(a0, a1)

    print("\n\n########### Done ! ############\n\n")
    print("New Location to visit: " + finalLat+ ", " + finalLon)

    time.sleep(3)
    open_browser(finalLat, finalLon)
    exit()
