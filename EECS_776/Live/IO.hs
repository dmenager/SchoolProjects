import Data.Maybe
import Data.Char

data Color = Red | Blue | Green
     deriving Show

data Pen = Pen Color
     deriving Show

--f :: [Color -> [Pen]
--f = map Pen


main :: IO ()
main = loop 99

type State = Int

loop :: State -> IO ()
loop n = do 
  putStrLn (show n ++ " bottles of irn bru on the wall")
  xs <- getLine
  case parse xs of 
    Nothing -> do
      putStrLn "Hu?"
      loop n
    Just cmd -> case next cmd n of
                  Nothing loop (n - m)


data Cmd = Smash Int | Replace Int
  deriving Show

parse :: String -> Maybe Cmd
parse xs = case words xs of
            ["smash", ns] | all isDigit ns -> Just (Smash (read ns))
            _ -> Nothing


-- Laws of Monads
-- -; M = M
-- M; - = M
-- (A;B);C = A;(B;C)
--
-- return :: a -> IO a
-- (>>=) :: IO a -> (a -> IO b) -> IO b ....thats the bind operator
--
