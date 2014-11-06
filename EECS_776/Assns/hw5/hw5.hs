{-# LANGUAGE GADTs, KindSignatures #-}

import Debug.Trace

data Tree :: * where
  Leaf :: Int          -> Tree
  Node :: Tree -> Tree -> Tree
  deriving(Show, Eq, Ord)

sumLeaves :: Tree -> Int
sumLeaves (Leaf i) =  i
sumLeaves (Node t1 t2) = (sumLeaves t1)  + (sumLeaves t2)

depth :: Tree -> Int
depth (Leaf _) = 0
depth (Node t1 t2) = 1 + max (depth t1) (depth t2)

getLeaves :: Tree -> [Int]
getLeaves (Leaf i) = [i]
getLeaves (Node t1 t2) = getLeaves t1 ++ getLeaves t2

balance :: Tree -> Tree
balance (Leaf i) = (Leaf i)
balance t = toBalance $ getLeaves t

toBalance :: [Int] -> Tree
--toBalance xs | traceShow ("toBalance", xs) False = undefined
toBalance [x] = (Leaf x)  
toBalance (x : xs) =  Node (toBalance (take ((length (x : xs)) `div` 2) (x : xs))) (toBalance (drop ((length (x : xs)) `div` 2) (x: xs)))

data Toppings :: * where
  Pinapple :: Toppings
  Mushroom :: Toppings
  RedOnions :: Toppings
  Spinach :: Toppings
  Cheese :: Toppings
  BellPepper :: Toppings
  Pepperoni :: Toppings
  CanadianBacon :: Toppings
  Bacon :: Toppings
  Sausage :: Toppings
  ItalianSausage :: Toppings
  Beef :: Toppings
  deriving(Show, Eq, Ord)

data Pizza :: * where
  Garnishes :: [Toppings] -> Pizza
  deriving (Show, Eq, Ord)

pizza = [Cheese, Bacon, Pepperoni :: Toppings]

pizza2 = [Cheese, BellPepper, Mushroom, Pinapple]
  
