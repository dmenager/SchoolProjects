data Tree :: * -> * where
  Leaf :: a -> Tree a
  Node :: Tree a -> Tree a -> Tree a
  deriving (Show, Eq, Ord)

terminals :: Tree a -> [a]
terminals (Leaf i) = [i]
terminals (Node t1 t2) = terminals t1 ++ terminals t2

--p1 :: Tree a -> Tree Int
--p1 (Leaf _) = (Leaf 1)
--p1 (Node t1 t2) = p1 t1 `Node` p1 t2
--p1 t = TreeMap (\ _ -> 1) t

p1 t = fmap (\ _ 1 :: Int) t

treeMap :: (a -> b) Tree a -> Tree b
treeMap f (Leaf a) = Leaf(f a)
treeMap f (Node t1 t2) = Node (treeMap f t1)
                              (treeMap f t2)

instance Functor Tree where
  --fmap :: (a -> b) -> Tree a -> Tree b  for instances, you don't normally
  --put down the type
  fmap f (Leaf a) = Leaf (f a)
  fmap f (Node t1 t2) = Node (fmap f t1)
                             (fmap f t2)
