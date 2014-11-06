-- get random value from list
elements ::[a] -> Gen a
elements xs = Gen (\ n -> xs !! (n `mod` length xs))
