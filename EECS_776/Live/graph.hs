type Graph = [(Int, [Int])]

--pure version
count :: Graph -> Int
count = length

empty :: Graph
empty = []

addNode :: Int -> Graph -> Graph
addNode n gs = (n , []) : gs

addEdge :: Int -> Int -> Graph
addEdge from to gs = [if n == from
                      then (n, to : es)
                      else (n, es)
                     | (n, es) <- gs ]


--Look at in-class code for monad version of this
