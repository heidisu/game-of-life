type cell = {
    x: int,
    y: int
};

type game;

let defaultGame: game;

let toggleCell: (game, cell) => game

let step: game => game;

let clear: game => game;

let getSize: game => int;

let getLivingCells: game => list(cell)