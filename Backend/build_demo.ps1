$files = @(
    "main.cpp",
    "Ngrams/manager/NGramManager.cpp",
    "Ngrams/manager/PredictionSession.cpp",
    "Ngrams/tipos/bigrams/Bigram.cpp",
    "Ngrams/tipos/unigrams/Unigrams.CPP",
    "utils/FileManager.cpp",
    "utils/TextBuilder.cpp",
    "utils/tokenizador.cpp"
)

Write-Host "Compiling Ngrams Demo..."
g++ $files -o demo.exe

if ($LASTEXITCODE -eq 0) {
    Write-Host "Compilation successful! Running demo..." -ForegroundColor Green
    ./demo.exe
} else {
    Write-Host "Compilation failed." -ForegroundColor Red
}
