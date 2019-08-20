import base64
import gzip

with open('dump.bin', 'rb') as f:
    raw = f.read()

b64 = base64.b64decode(raw).decode('ascii').replace('\00', '')

b64_payload = b64.split('FromBase64String("')[1].split('"')[0]
gzip_payload = base64.b64decode(b64_payload)
payload = gzip.decompress(gzip_payload)
#print(payload)

with open('result.txt', 'w') as f:
	f.write(payload.decode('ascii'))

