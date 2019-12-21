# proxy-switcher
A seriously hack-ish system proxy switcher module for Node.js. For the moment, this only works on GNU/Linux.

## Supported features
HTTP, HTTPS, SOCKS, and FTP proxies are all supported.
This module uses gsettings to configure the system proxy on GNU/Linux systems. Authentication is not currently supported,
becasue there are no authentication settings in gsettings for any proxy type other than HTTP. I do intend
to support at least HTTP authentication in the future.

## Installing (Ubuntu)
```shell
$ sudo apt install build-essential libglibmm-2.4-dev
$ npm i
```

## Usage
```javascript
const ps = require('proxy-switcher')

ps.setProxy(ps.ProxyType.HTTP, 'http://myhost.mydomain', 8080)
ps.setProxyEnabled(true)
```

That's it!

