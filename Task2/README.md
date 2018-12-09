
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


### Error codes
Besides all *standard* HTTP status codes we would want to implement some messages to few other of them.

|Code |Message                                          |
|---  |---                                              |
|---  |**Client**                                       |
|406  | Not accepted, valid request?                    |
|408  | Request Timeout, client might not be running.   |
|---  |**Server**                                       |
|501  | Not implemented, check documentation.           |
|503  | Service not available. If server is busy.       |

#### Example

This is how the client "saves" the json objects now.

```json
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
{ "timestamp":"%y-%m-%d HH:mm:ss", "rssi":"-XXX", "address":"xx:xx:xx:xx:xx:xx" }
```

* Timestamp: When probe was observed.
* Rssi: Signal strength. [Resource on RSSI](https://www.metageek.com/training/resources/understanding-rssi.html).
* address: MAC address of observed device.
