module EvilEncryption where

{

import Data.Char;
import IO;

{- Prepare string for encryption. -}
prepare :: String -> String;
prepare = (filter (`elem` (['a'..'z'])))
             . (map toLower);

{- Transform a 26-letter permutation key into
an actual permutation of the alphabet. -}
permutation :: String -> Char -> Char;
permutation s x = s !! (ord x - 97);

{- Encrypt the prepared input using the key. -}
encrypt :: String -> String -> String;
encrypt key x = map (\i -> (iterate
      (permutation key) (x !! i)) !! i)
       (take (length x) (enumFrom 0));

{- Encrypt a file. -}
main = do {
     putStrLn "Enter file to encrypt (full ";
     putStrLn "path): ";
     path <- getLine;
     putStrLn "Enter name of encrypted file: ";
     pathenc <- getLine;
     putStrLn "Enter key (permutation of the ";
     putStrLn "alphabet as 26-letter string): ";
     key <- getLine;
     x <- readFile path;
     y <- return (encrypt key (prepare x));
     writeFile pathenc y;
     putStr y;
}

}










