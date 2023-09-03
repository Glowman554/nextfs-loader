let load_addr = prompt("Load address (hex)? ");

let str = "M" + load_addr + " ";
for (const i of Deno.readFileSync(prompt("File? "))) {
    str += i.toString(16) + " ";
}
str += "\\\nG" + load_addr;
console.log(str);