{-# LANGUAGE TemplateHaskell #-}

module Main where
import Printf ( pr )
import CxxyLib

main :: IO ()
main = do
  putStrLn ( $(pr "Hello From Template Haskell!") )
  talkToCxx
