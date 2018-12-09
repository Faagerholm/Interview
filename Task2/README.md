
## API documentation

### Client 

`GET /active		get status of device. respons is boolean true/false.`  
`GET /peak/all		get peak on all available collected probes.`  
`GET /pop/all		get all json objects collected, clearing the small memory of the device.`  

`POST /activate		post a request to activate the device, expected a boolean. { "activate": true/false }`  	

### Server
`GET /active		get status of server. respons is boolean true/false.`  

#### Example

This is how the client "saves" the json objects now.

```json
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
```
