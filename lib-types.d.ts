import { Buffer } from "nbind/dist/shim";

export class NBindBase { free?(): void }

export class Box extends Container {
	/** Box(); */
	constructor();

	/** Box(uint16_t, Vec3usi, Vec3usi); */
	constructor(p0: number, p1: Vec3usi, p2: Vec3usi);

	/** Box(uint16_t, Vec3usi); */
	constructor(p0: number, p1: Vec3usi);

	/** bool packed; */
	packed: boolean;

	/** Vec3usi coordinates; */
	coordinates: Vec3usi;

	/** Vec3usi packedDimensions; */
	packedDimensions: Vec3usi;
}

export class BoxInfo extends Container {
	/** BoxInfo(); */
	constructor();

	/** BoxInfo(uint16_t, Vec3usi); */
	constructor(p0: number, p1: Vec3usi);

	/** BoxInfo(uint16_t, Vec3usi, uint16_t); */
	constructor(p0: number, p1: Vec3usi, p2: number);

	/** uint16_t count; */
	count: number;

	/** uint16_t packed; */
	packed: number;
}

export class Container extends NBindBase {
	/** Container(); */
	constructor();

	/** Container(uint16_t, Vec3usi); */
	constructor(p0: number, p1: Vec3usi);

	/** uint16_t label; */
	label: number;

	/** Vec3usi dimensions; */
	dimensions: Vec3usi;

	/** uint32_t volume; */
	volume: number;
}

export class Pack extends NBindBase {
	/** Pack(); */
	constructor();

	/** Pack(Container, std::vector<Box>); */
	constructor(p0: Container, p1: Box[]);

	/** Container container; */
	container: Container;

	/** std::vector<Box> boxes; */
	boxes: Box[];
}

export class Packer extends NBindBase {
	/** Packer(); */
	constructor();

	/** std::vector<Pack> pack(std::vector<Container>, std::vector<BoxInfo>); */
	pack(p0: Container[], p1: BoxInfo[]): Pack[];
}

export class Vec3usi extends NBindBase {
	/** Vec3usi(); */
	constructor();

	/** Vec3usi(uint16_t, uint16_t, uint16_t); */
	constructor(p0: number, p1: number, p2: number);

	/** uint16_t x; */
	x: number;

	/** uint16_t y; */
	y: number;

	/** uint16_t z; */
	z: number;
}
