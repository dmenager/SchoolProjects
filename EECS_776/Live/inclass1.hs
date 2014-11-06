{-# LANGUAGE GADTs, KindSignatures #-}

data Expr = Lit Int | Add Expr Expr | Div Expr Expr
  deriving (Show, Eq, Ord)

eval :: Expr -> Maybe Int
eval (Lit n) = Just n
eval (Add l r) = case (eval l) of
                  Nothing -> Nothing
                  Just val -> case (eval r ) of
                    Nothing -> Nothing
                    Just val2 -> Just $ val + val2
eval (Div l r) = case (eval l) of 
  Nothing -> Nothing
  Just val -> case (eval r) of
    Nothing -> Nothing
    Just val2 -> case val2 of
      0 -> Nothing
      _ -> Just $ val `div` val2

main :: IO() --unit signifies no return type
main = do x <- getLine
  putStrLn ""
  putChar x
