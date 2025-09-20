import fsPromise from 'fs/promises';
import path from 'path';
import util from 'util';

const args = process.argv.slice(2);
const parsedArgs = util.parseArgs({
    args,
    allowPositionals: true,
    options: {
        // name: {
        //     type: 'string',
        //     short: 'n',
        //     default: 'MyClass'
        // },
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

function splitNamespace(typeName) {
    const parts = typeName.split('::');
    const basename = parts.pop();
    const namespace = parts;
    return { basename, namespace };
}

function getIncludeGuard(typeName) {
    const { basename, namespace } = splitNamespace(typeName);
    const snakeCaseName = toSnakeCase(basename);
    return [
        ...namespace.map(ns => ns.toUpperCase()),
        snakeCaseName.toUpperCase(),
        'HH'
    ].join('_');
}

function getHeaderPath(name, {
    includeDir
} = {}) {
    includeDir ??= true;

    const { namespace, basename } = splitNamespace(name);
    const snakeCaseName = toSnakeCase(basename);
    if (includeDir) {
        return path.join(headerDir, ...namespace, `${snakeCaseName}.hh`);
    } else {
        return path.join(...namespace, `${snakeCaseName}.hh`);
    }
}

function getSourcePath(name, {
    includeDir
} = {}) {
    includeDir ??= true;

    const { namespace, basename } = splitNamespace(name);
    const snakeCaseName = toSnakeCase(basename);
    if (includeDir) {
        return path.join(srcDir, ...namespace, `${snakeCaseName}.cc`);
    } else {
        return path.join(...namespace, `${snakeCaseName}.cc`);
    }
}

function generateHeader({
    name, type
}) {
    const { namespace, basename } = splitNamespace(name);
    const includeGuard = getIncludeGuard(name);
    let content = '';
    content += `#ifndef ${includeGuard}\n`;
    content += `#define ${includeGuard}\n\n`;
    if (namespace.length > 0) {
        content += `namespace ${namespace.join('::')} {\n`;
    }

    if (type === 'struct') {
        // Value-semantic type
        content += `struct ${basename} final {\n`;
        content += `};\n\n`;
    } else {
        // Reference-semantic type
        content += `class ${basename} {\n`;
        content += `public:\n`;
        content += `    explicit ${basename}() noexcept = default;\n`;
        content += `    ${basename}(${basename}&&) noexcept = default;\n`;
        content += `    virtual ~${basename}() noexcept = default;\n`;
        content += `\n`;
        content += `    ${basename}(const ${basename}&) = delete;\n`;
        content += `    ${basename}& operator=(const ${basename}&) = delete;\n`;
        content += `    ${basename}(${basename}&&) = default;\n`;
        content += `};\n`;
    }

    if (namespace.length > 0) {
        content += `} // namespace ${namespace.join('::')}\n`;
    }
    content += `#endif // ${includeGuard}\n`;
    return content;
}

function generateSource({ name, type }) {
    const { namespace, basename } = splitNamespace(name);
    let content = '';
    const headerPath = getHeaderPath(name, { includeDir: false });
    content += `#include "${headerPath}"\n`;
    content += `\n`;
    if (namespace.length > 0) {
        content += `namespace ${namespace.join('::')} {\n`;
    }

    if (namespace.length > 0) {
        content += `} // namespace ${namespace.join('::')}\n`;
    }
    return content;
}

const className = parsedArgs.positionals[0];
const type = parsedArgs.values.type;
const srcDir = parsedArgs.values.srcDir;
const headerDir = parsedArgs.values.headerDir;

const headerPath = getHeaderPath(className);
const sourcePath = getSourcePath(className);

const headerDirPath = path.dirname(headerPath);
const sourceDirPath = path.dirname(sourcePath);

await fsPromise.mkdir(headerDirPath, { recursive: true });
await fsPromise.mkdir(sourceDirPath, { recursive: true });

const headerContent = generateHeader({ name: className, type: type });
const sourceContent = generateSource({ name: className, type: type });

await fsPromise.writeFile(headerPath, headerContent, 'utf-8');
await fsPromise.writeFile(sourcePath, sourceContent, 'utf-8');

console.log(`Created ${headerPath} and ${sourcePath} `);