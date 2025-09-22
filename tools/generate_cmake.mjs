import fsPromises from "fs/promises";

const args = process.argv.slice(2);
const outputPath = args[0] || "CMakeLists.txt";

let content = await fsPromises.readFile(outputPath, "utf-8");

let index = 0;

while (index < content.length) {
    const beginMarker = /# include (.*)\n/;
    const endMarker = "# end";

    const beginMatch = beginMarker.exec(content.slice(index));
    if (!beginMatch) break;

    const beginIndex = index + beginMatch.index;
    const pattern = beginMatch[1].trim();
    const endIndex = content.indexOf(endMarker, beginIndex);
    if (endIndex === -1) {
        console.error(`End marker not found after begin marker at index ${beginIndex}`);
        process.exit(1);
    }

    const sources = await Array.fromAsync(fsPromises.glob(pattern));
    sources.sort();

    const before = content.slice(0, beginIndex + beginMatch[0].length).trimEnd();
    const after = content.slice(endIndex).trimStart();

    const sourceLines = sources.map(src => `  ${src}`);
    content = `${before}\n${sourceLines.join("\n")}\n${after}`;

    index = endIndex + endMarker.length;
}

await fsPromises.writeFile(outputPath, content, "utf-8");