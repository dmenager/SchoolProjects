type String = [Char]      -- String :: *
type Pair a = (a, a)      -- Pair :: * -> *
                             -- Pair Int :: * <-- instantiation

data Bool = False | True  
data Maybe a = Nothing | Just a       -- Nothing :: Maybe a
                                      -- Just :: a -> Maybe a

data Maybe :: * -> * where
  Nothing :: Maybe a
  Just :: a -> Maybe a

-- Polymorphic tree
data Tree a = Leaf a |  Node (Tree a) (Tree a)

-- Or
data Tree2 :: * -> * where
  Leaf :: a -> (Tree2 a)
  Node :: Tree2 a ->  Tree2 a -> Tree2 a   -- Node takes two trees, returns a tree


newtype Slist = SList[Int] -- First SList is type. Second is Constructor...only 1 constructor, and must only have 1 arg...Less powerful than data
SList :: [Int] -> Slist
--Instances
(==) :: Eq a => a -> a -> Bool

(1 :: Int) == 2   -- Looks in the dictionary to see if Eq is defined for Int first
                  -- Eq a ~= (a -> a -> Bool, a -> a -> Bool) first function
                        -- is equality. Second function is negation of it
                  -- Eq Int ~= (Int -> Int -> Bool, Int -> Int -> Bool)

instance Eq a => Eq (Tree a) where  -- If you can show me Eq a exists, then I can do Eq (Tree a) 
(==) (Leaf a) (Leaf b) = a == b
(==) (Node a1 a2) (Node b1 b2) = (a1 == b1) && (a2 == b2) -- type of a1, etc is a tree. So it recurses on iteslf
(==) _ _ = False

f :: Pair Int -> Int
f (a,b) = a + b
