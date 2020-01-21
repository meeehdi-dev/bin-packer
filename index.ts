import * as nbind from "nbind";
import * as LibTypes from './lib-types';

const lib = nbind.init<typeof LibTypes>().lib;

const packer = new lib.Packer();
const packs = packer.pack([], []);

console.log(packs);