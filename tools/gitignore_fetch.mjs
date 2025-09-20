import fsPromises from 'fs/promises';

const args = process.argv.slice(2);
const gitignorePath = args[0];

if (!gitignorePath) {
    console.error('Usage: node gitignore_fetch.mjs <path-to-gitignore>');
    process.exit(1);
}

let content = await fsPromises.readFile(gitignorePath, 'utf-8');
let lines = content.split('\n');

const includeRegex = /^# include (https?:\/\/\S+)$/;

for (let i = lines.length - 1; i >= 0; i--) {
    let line = lines[i].trim();

    let match = line.match(includeRegex);

    if (match) {
        let url = match[1];

        console.log(`Fetching and including from: ${url}`);

        const fetched = await (await fetch(url)).text();
        let fetchedLines = fetched.split('\n');
        lines.splice(i, 1, [
            `# begin ${url}`,
            ...fetchedLines,
            `# end ${url}`]);
    }
}

content = lines.flat().join('\n');
await fsPromises.writeFile(gitignorePath, content, 'utf-8');