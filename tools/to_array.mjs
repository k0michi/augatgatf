/**
 * Converts a binary file to a C++ std::array of std::byte and prints it to stdout.
 */

import util from 'util';
import fsPromises from 'fs/promises';
import path from 'path';

const args = process.argv.slice(2);
const parsedArgs = util.parseArgs({
  args,
  allowPositionals: true,
});

const file = parsedArgs.positionals[0];
if (!file) {
  console.error('Usage: node to_array.mjs <file>');
  process.exit(1);
}

let content = await fsPromises.readFile(file);
let source = '';

source += `const std::array<std::byte, ${content.length}> data = {\n`;
for (let i = 0; i < content.length; i++) {
  source += `std::byte(${content[i]}), `;
}
source += '};\n';
console.log(source);