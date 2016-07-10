'require strict'

var i2c = require('i2c');
var address = 0x18;
var wire = new i2c(address, {device: '/dev/i2c-2'});

console.log(wire.scan());
