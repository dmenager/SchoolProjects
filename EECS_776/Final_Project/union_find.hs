{-# LANGUAGE GADTs, KindSignatures, OverloadedStrings #-}
import Data.List
import Graphics.Blank hiding (fillStyle, strokeStyle)
import Graphics.Blank.Style
import qualified Data.Text as T
import Data.Text(Text)
import Control.Applicative
import Control.Monad
import Text.Printf
-- -----------------------------------------------------------------
newtype State ::  * -> (* -> *) where
  State_ :: (s-> (a,s)) -> State s a

instance Monad (State s) where
  -- return :: a -> State s a
  return a = State_ (\ st0 -> (a,st0))
  -- (>>=) :: State s a -> (a -> State s b) -> State s b
  (State_ f) >>= k = State_ (\ st0 ->
        -- f :: (s -> (a,s))
          let (a,st1) = f st0 in
          case (k a) of
            State_ g -> g st1)

                     
get :: State s s
get = State_ (\ st -> (st,st))

set :: s -> State s ()
set n  = State_ (\ st -> ((), n))

runST :: s -> State s a -> a
runST s (State_ f) = fst (f s)

execST :: s -> State s a -> s
execST s (State_ f) = snd (f s)
-- -------------------------------------------------

type DS = [Int]
type Disjoint a = State DS a
type Color = Text

init_ds :: DS
init_ds = []

-- Finds root representative for x
find_ds :: Int -> DS -> Int 
find_ds x adt = if adt !! x == x then adt !! x else find_ds (adt !! x) adt

-- Unions two sets together
fast_union :: Int -> Int -> DS -> DS
fast_union x y adt = take (find_ds x adt) adt ++ [find_ds y adt] ++ drop(find_ds x adt + 1) adt

addNode :: Int-> DS -> DS
addNode p ds = ds ++ [p]

runDisjoint :: DS -> Disjoint Int -> Int
runDisjoint = runST

findDS :: Int -> Disjoint Int
findDS x = do
  s <- get 
  return (find_ds x s)

funion :: Int -> Int -> Disjoint ()
funion x y = do
  s <- get
  set (fast_union x y s)
  return ()

addNodeM :: Int -> Disjoint Int
addNodeM r = do
  s <- get
  set(addNode r s)
  return r

main :: IO ()
main = do
     let x = execST init_ds $ do 
                               y <- get
                               addNodeM 1 
                               addNodeM 2 
                               addNodeM 3
                               funion 0 1
     blankCanvas 3000 $ \ context ->
         send context $ do
              forM_ x  $ \ item -> do
                      let offset = 200
                      let centerX = fromIntegral $ 100 + offset * (item - 1)
                      let yoffset = 100
                      let centerY = height context / 2
                      let radius = 79
                      let r_base = 25
                      let g_base = 100
                      let b_base = 75
                                   
                      beginPath()
                      fillStyle $ rgb (r_base * fromIntegral item) (g_base * fromIntegral item) (b_base * fromIntegral item) 
                      arc(centerX, centerY, radius, 0, 2 * pi, False)
                      lineWidth 5
                      strokeStyle ("black":: Text)
                      font "30pt Calibri"
                      fillText ((T.pack $  show item), centerX, centerY)
                      stroke()
                      closePath ()
                      fill()
