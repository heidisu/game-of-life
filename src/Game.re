type cell = {
    x: int,
    y: int
};


type size = {
    width: int,
    height: int
}

module Cell =  {
    type t = cell
    let compare = (c1, c2) => {
        switch(compare(c1.x, c2.x)){
            | 0 => compare(c1.y, c2.y)
            | c => c
        };
    };
};

module CellSet = Set.Make(Cell)


type game = {
    size: size,
    livingCells: CellSet.t
};

let glider = [ {x: 2, y: 1},  {x: 3, y: 2},  {x: 1, y: 3},  {x: 2, y: 3},  {x: 3, y: 3}]

let defaultGame = { size: { width: 25, height: 20 }, livingCells: CellSet.of_list(glider) };

let toggleCell = (game, cell) => {
    let livingCells = 
        if (CellSet.exists((c) =>  c == cell, game.livingCells)){
            CellSet.remove(cell, game.livingCells);
        } else {
            CellSet.add(cell, game.livingCells);
        };
    { ...game,  livingCells: livingCells }
};

let isNeighbours = (c1, c2) : bool => {
    c1 != c2 &&
    ( c1.x == c2.x || c1.x == c2.x - 1 || c1.x == c2.x + 1) &&
    ( c1.y == c2.y || c1.y == c2.y - 1 || c1.y == c2.y + 1)
};

let numberOfLivingNeighbours = (cell: CellSet.elt, livingCells: CellSet.t) => {
    livingCells
    |> CellSet.filter((c) => isNeighbours(c, cell))
    |> CellSet.cardinal
};


let getNeighbours = (cell) : list(cell) => {
    [
        { x: cell.x - 1, y: cell.y - 1},
        { x: cell.x - 1, y: cell.y},
        { x: cell.x - 1, y: cell.y + 1},
        { x: cell.x, y: cell.y - 1},
        { x: cell.x, y: cell.y + 1},
        { x: cell.x + 1, y: cell.y - 1},
        { x: cell.x + 1, y: cell.y},
        { x: cell.x + 1, y: cell.y + 1}
    ];
};

let step = (game: game) => {
    // set does not have map and flatten...
    let neighbours = 
        game.livingCells
        |> CellSet.elements
        |> List.map (cell => getNeighbours(cell))
        |> List.flatten
        |> CellSet.of_list
    let deadNeighbours = CellSet.diff(neighbours, game.livingCells);
    let deadBecomingAlive = 
        deadNeighbours 
        |> CellSet.filter(cell => {numberOfLivingNeighbours(cell, game.livingCells) == 3});
    let liveCellsContinueLiving = 
        game.livingCells
        |> CellSet.filter(cell => {
            let liveNeighbours = numberOfLivingNeighbours(cell, game.livingCells)
            liveNeighbours == 2 || liveNeighbours == 3
        });
    
    { size: game.size, livingCells: CellSet.union(deadBecomingAlive, liveCellsContinueLiving) }
};

let clear = (game: game) => {
    { size: game.size, livingCells: CellSet.empty }
};

let getSize = (game: game) => game.size;

let getLivingCells = (game: game) => CellSet.elements(game.livingCells);