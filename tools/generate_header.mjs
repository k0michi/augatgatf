import fsPromises from "fs/promises";
import path from "path";
import util from "util";

const args = process.argv.slice(2);

const parsedArgs = util.parseArgs({
  args,
  allowPositionals: true,
});

const outputPath = parsedArgs.positionals[0] || "target.cpp";
const parent = path.dirname(outputPath);

let content = await fsPromises.readFile(outputPath, "utf-8");
content = content.replace(/\r\n/g, "\n");

let index = 0;

while (index < content.length) {
  const beginMarker = /\/\/ include (.*)\n/;
  const endMarker = "// end";

  const beginMatch = beginMarker.exec(content.slice(index));
  if (!beginMatch) break;

  const beginIndex = index + beginMatch.index;
  const pattern = beginMatch[1].trim();
  const endIndex = content.indexOf(endMarker, beginIndex);
  if (endIndex === -1) {
    console.error(`End marker not found after begin marker at index ${beginIndex}`);
    process.exit(1);
  }

  let headers = await Array.fromAsync(fsPromises.glob(pattern, { cwd: parent }));
  headers = headers.map(src => src.replace(/\\/g, "/"));
  headers.sort();

  const before = content.slice(0, beginIndex + beginMatch[0].length).trimEnd();
  const after = content.slice(endIndex).trimStart();

  const includeLines = headers.map(h => `#include "${h}"`);
  content = `${before}\n${includeLines.join("\n")}\n${after}`;

  index = endIndex + endMarker.length;
}

await fsPromises.writeFile(outputPath, content, "utf-8");