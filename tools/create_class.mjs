import fsPromise from 'fs/promises';
import { type } from 'os';
import path from 'path';
import util from 'util';

const args = process.argv.slice(2);
const parsedArgs = util.parseArgs({
    args,
    allowPositionals: true,
    options: {
        name: {
            type: 'string',
            short: 'n',
            default: 'MyClass'
        },
        type: {
            type: 'string',
            short: 't',
            choices: ['class', 'struct'],
            default: 'class'
        },
        srcDir: {
            type: 'string',
            short: 's',
            default: 'src'
        },
        headerDir: {
            type: 'string',
            short: 'd',
            default: 'include'
        },
    }
});

// AClass -> a_class
// IOStream -> io_stream
function toSnakeCase(str) {
    return str
        .replace(/([a-z0-9])([A-Z])/g, '$1_$2')
        .replace(/([A-Z])([A-Z][a-z])/g, '$1_$2')
        .toLowerCase();
}

function splitNamespace(className) {
    const parts = className.split('::');
    const name = parts.pop();
    const namespace = parts;
    return { name, namespace };
}

const className = parsedArgs.values.name;
const classType = parsedArgs.values.type;
const srcDir = parsedArgs.values.srcDir;
const headerDir = parsedArgs.values.headerDir;

const { name, namespace } = splitNamespace(className);
const snakeCaseName = toSnakeCase(name);

const headerPath = path.join(headerDir, ...namespace, `${snakeCaseName}.hh`);
const sourcePath = path.join(srcDir, ...namespace, `${snakeCaseName}.cc`);

const headerGuard = [
    ...namespace.map(ns => ns.toUpperCase()),
    snakeCaseName.toUpperCase(),
    'HH'
].join('_');

const headerDirPath = path.dirname(headerPath);
const sourceDirPath = path.dirname(sourcePath);

await fsPromise.mkdir(headerDirPath, { recursive: true });
await fsPromise.mkdir(sourceDirPath, { recursive: true });

const headerContent = `#ifndef ${headerGuard}
#define ${headerGuard}

${namespace.length > 0 ? `namespace ${namespace.join(' {\nnamespace ')} {\n` : ''}${classType} ${name} {
public:
    ${name}() = default;
    ~${name}() = default;

private:

};${namespace.length > 0 ? '\n' + '}'.repeat(namespace.length) + '\n' : ''}

#endif // ${headerGuard}
`;

const sourceContent = `#include "${path.posix.relative(sourceDirPath, headerPath)}"

${namespace.length > 0 ? `namespace ${namespace.join(' {\nnamespace ')} {\n` : ''}

// Implement ${name} methods here

${namespace.length > 0 ? '\n' + '}'.repeat(namespace.length) + '\n' : ''}
`;

await fsPromise.writeFile(headerPath, headerContent, 'utf-8');
await fsPromise.writeFile(sourcePath, sourceContent, 'utf-8');

console.log(`Created ${headerPath} and ${sourcePath}`);