type cell = {
    x: int,
    y: int
};

type size = {
    width: int,
    height: int
}

type game;

let defaultGame: game;

let toggleCell: (game, cell) => game

let step: game => game;

let clear: game => game;

let getSize: game => size;

let getLivingCells: game => list(cell)

let setWidth: (game, int) => game;

let setHeight: (game, int) => game;