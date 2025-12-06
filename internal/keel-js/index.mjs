import { createRequire } from 'module';
const require = createRequire(import.meta.url);
const addon = require('./build/Release/keel-js');

console.log(addon.getVersion());
console.log(addon.getRevision());