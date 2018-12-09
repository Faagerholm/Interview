
## API documentation

### Client 

|Request  |URI            |Comment                                                    |Example/Variables          |
|---      |---            |---                                                        |---                        |
|**GET**|/active          |Get status of device. respons is boolean true/false.       |                           |
|**GET**|/peek/all        |Peek on all available collected probes.                    |                           |
|**GET**|/peek/{from}/{to}    |Peek on available collected probes within timestamp.       |/peak/2018-12-01/2018-12-10|
|**GET**|/pop/all         |Get all available json objects collected.                  |                           |
|**GET**|/pop/{from}/{to}     |Get all available json objects collected within timestamp. |/pop/2018-12-01/2018-12-10 |
|**POST**|/activate/      |Post a request to change device status. expected boolean   |{ "activate": true/false }  |

### Server


|Request  |URI            |Comment                                                    |Example/Variables          |
|---      |---            |---                                                        |---                        |
|**GET**|/active          |Get status of server. respons is boolean true/false.       |                           |

#### Example

This is how the client "saves" the json objects now.

```json
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
```
