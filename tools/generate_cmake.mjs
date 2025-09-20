import fsPromises from "fs/promises";

const args = process.argv.slice(2);
const outputPath = args[0] || "CMakeLists.txt";

const sources = await Array.fromAsync(fsPromises.glob("src/**/*.cc"));

let content = await fsPromises.readFile(outputPath, "utf-8");

const beginMarker = "# begin";
const endMarker = "# end";

const beginIndex = content.indexOf(beginMarker);
const endIndex = content.indexOf(endMarker, beginIndex + beginMarker.length);

if (beginIndex === -1 || endIndex === -1) {
    console.error(`Markers "${beginMarker}" and "${endMarker}" not found in ${outputPath}`);
    process.exit(1);
}

const before = content.slice(0, beginIndex + beginMarker.length).trimEnd();
const after = content.slice(endIndex).trimStart();

const sourceLines = sources.map(src => `    ${src}`);
const newContent = `${before}\n${sourceLines.join("\n")}\n${after}`;

await fsPromises.writeFile(outputPath, newContent, "utf-8");
console.log(`Updated ${outputPath} with ${sources.length} source files.`);