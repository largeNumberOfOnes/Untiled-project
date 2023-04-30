
type Matrix = [Float]


-- isMatrixSquare :: Matrix -> Bool
isMatrixSquare matrix = if (interim matrix) == (length matrix) then True else False where
-- isMatrixSquare matrix = (interim matrix) where
    -- interim matrix = 1
    interim (h:[]) = length h
    interim (h:oth) = let l = length h in if l == interim oth then l else -1


main = print $ isMatrixSquare matrix where
    matrix = [[1,2,3], [4,2,8], [8,6,2], [2,3,4]]

