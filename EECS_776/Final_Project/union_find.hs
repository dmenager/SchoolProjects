{-# LANGUAGE GADTs, KindSignatures #-}
import Data.List

type DS = [Int]

init_ds :: Int -> DS
init_ds num = [0 .. num - 1]

-- Finds root representative for x
find_ds :: Int -> DS -> Int 
find_ds x adt = if adt !! x == x then adt !! x else find_ds (adt !! x) adt

fast_union :: Int -> Int -> DS -> DS
fast_union x y adt = take (find_ds x adt) adt ++ [find_ds y adt] ++ drop(find_ds x adt + 1) adt
       
