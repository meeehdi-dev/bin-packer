import * as nbind from "nbind";
import * as PackerTypes from './lib/packer';

const lib = nbind.init<typeof PackerTypes>().lib;

const packer = new lib.Packer();
const packs = packer.pack([], []);

console.log(packs);
