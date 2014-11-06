import Data.List
import Data.Maybe

type StudentRecord = (String, Integer, [(String, Char)])
type StudentRecords = [StudentRecord]

letters = "FDCBA"

gpa :: StudentRecord -> Double
gpa (a,b, (c:cs)) = fromIntegral( (foldr (+) 0 ( map (\ x -> fromJust ( elemIndex x  letters ) ) ( map (\  ( x, y )  ->  y ) (c:cs) ) ) ) ) / fromIntegral (length (c:cs))   


toTuple :: String -> Double -> (String, Double)
toTuple a b = (a, b)

firstT (a, _, _) = a

summary :: StudentRecords -> [(String, Double)]
summary ((a : b : (c:cs))) = map (\ xs -> toTuple (firstT xs) (gpa xs)) ((a : b : (c:cs)))

students = [("Joe Dana", 1345, [("EECS 776", 'A'), ("EECS 123", 'B')]), ("Roger Rabbit", 3544, [("EECS 662", 'C')]), ("Fred Flintstone", 4734, [("EECS 448", 'A'), ("EECS 368", 'B'), ("EECS 388", 'C')]), ("Eddie Haskell", 3452, [("EECS 168", 'A')]), ("Waldo", 1422, [("EECS 108", 'A'),("EECS 268", 'D')])]
