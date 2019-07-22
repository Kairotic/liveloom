module WWL where

import qualified System.Hardware.Serialport as SP
import Control.Concurrent
import Control.Concurrent.MVar
import qualified Data.ByteString.Char8 as B
import Text.Printf

data Weave = Weave {wBits :: [Bool],
                    wRow :: Int,
                    wSend :: (Weave -> IO ()),
                    wSerialPort :: SP.SerialPort
                   }

heddleMap = [0,2,4,6,8,10,12,14,1,3,5,7,9,11,13,15]


--1 2 3 4  5  6 7 8 9 10 11 12 13 14 15 16
--3 5 7 9 11 13 4 6 8 10 12 14 16 15  1  2

up :: Bool
up = True

down :: Bool
down = False

width = 16
height = 12

every :: Int -> ([a] -> [a]) -> [a] -> [a]
every _ _ [] = []
every 0 _ xs = xs
-- take width added after the application of `f` to make sure transformations don't go outside the row
every n f xs = (take width $ f $ take width xs) ++ (take (width * (n-1)) $ drop width xs) ++ every n f (drop (width*n) xs)

{-
twill :: Int -> [a] -> [a]
twill 0 xs = xs
twill n xs = twill' 0 xs
  where twill' _ [] = []
        twill' i xs = (drop shift row ++ take shift row) ++ twill' (i+1) (drop width xs)
          where row = (take width xs)
                shift = (i * n) `mod` width
-}
twill :: Int -> [a] -> [a]
twill 0 xs = xs
twill _ [] = []
twill n xs = take width row ++ twill n (drop width row)
  where row = drop n xs


-- take width added after the applicaiton of `f` to make sure transformations don't go outside the row
each n f xs = (take width $ f $ take width xs) ++ (take (width * (n-1)) $ drop width xs) ++ every n f (drop (width*n) xs)

shift :: [a] -> [a]
shift [] = []
shift (x:xs) = xs ++ [x]

rev :: [a] -> [a]
rev [] = []
rev xs = (reverse $ take width xs) ++ (rev $ drop width xs)

invert :: [Bool] -> [Bool]
invert = map not

double :: [a] -> [a]
double [] = []
double (x:xs) = (x:x:double xs)

backforth :: [a] -> [a]
backforth = every 2 reverse

weave :: [Bool] -> [String]
weave bits = weave' True bits
  where
    weave' _ [] = []
    -- even
    weave' _ bits' = (showRow $ take width bits') : (weave' False $ drop width bits')
    -- odd
    -- lets not go back and forth for now
    -- weave' False bits' = (reverse $ showRow $ take width bits') : (weave' True $ drop width bits')
    showRow = map (\x -> if x then '1' else '0')

start :: String -> IO (MVar Weave)
start dev = do out <- SP.openSerial dev (SP.defaultSerialSettings {SP.commSpeed = SP.CS9600})
               mvSend <- newEmptyMVar
               mvWeave <- (newMVar $ Weave {wBits = cycle [up,down],
                                            wRow = -1,
                                            wSend = putMVar mvSend,
                                            wSerialPort = out
                                           }
                          )
               forkIO $ sendLoop mvSend
               return mvWeave

startFs :: String -> IO ([Bool] -> IO (), IO (), IO (), IO (), IO ())
startFs dev = do w <- start dev
                 -- return (swapMVar w >> \_ -> return (), makeNext w, makeUndo w, makeAgain w)
                 return (makeSet w, makeNext w, makeUndo w, makeAgain w, makeShow w)


makeNext :: MVar Weave -> IO ()
makeNext mvWeave = do w <- takeMVar mvWeave
                      let w' = w {wRow = (wRow w) + 1}
                      (wSend w') w'
                      putMVar mvWeave w'
                      makeShow mvWeave

makeUndo :: MVar Weave -> IO ()
makeUndo mvWeave = do w <- takeMVar mvWeave
                      let w' = w {wRow = (wRow w) - 1}
                      (wSend w') w'
                      putMVar mvWeave w'
                      makeShow mvWeave

makeAgain :: MVar Weave -> IO ()
makeAgain mvWeave = do w <- readMVar mvWeave
                       (wSend w) w
                       makeShow mvWeave

makeSet :: MVar Weave -> [Bool] -> IO ()
makeSet mvWeave bits = do w <- takeMVar mvWeave
                          putMVar mvWeave $ w {wBits = bits}
                          makeShow mvWeave
                          return ()

makeShow :: MVar Weave -> IO ()
makeShow mvWeave = do w <- readMVar mvWeave
                      putStrLn ""
                      putStrLn $ showWeave w
                      return ()

showWeave :: Weave -> String
showWeave w = concatMap showRow rows
  where showRows = 12
        start = max 0 (wRow w - (showRows-6))
        toLine '1' = '|'
        toLine '0' = '–'
        toLine  x = x
        rows = take showRows $ drop start $ zip (map (map toLine) $ weave (wBits w)) [0..]
        showRow (s, i) | i == wRow w = printf "%4d > %s <\n" i s
                       | otherwise   = printf "%4d : %s\n" i s

sendLoop :: MVar Weave -> IO ()
sendLoop mv = loop
  where loop = do w <- takeMVar mv
                  go w
                  loop

go :: Weave -> IO ()
go w = do let r = (cycle $ weave $ wBits w) !! wRow w
              r' = map (r !!) heddleMap
          SP.send (wSerialPort w) $ B.pack $ r' ++ "x\n"
          return ()
