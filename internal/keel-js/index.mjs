import { createRequire } from 'module';
const require = createRequire(import.meta.url);
const addon = require('./build/Release/hello-addon');

console.log(addon.add(3, 5));