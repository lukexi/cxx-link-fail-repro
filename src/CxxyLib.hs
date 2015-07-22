{-# LANGUAGE ForeignFunctionInterface #-}

module CxxyLib where

foreign import ccall "talkToCxx" talkToCxx :: IO ()