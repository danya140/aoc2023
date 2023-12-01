
cp -r day_template day_$1 # copy template
cd day_$1

# Read cookies.txt from file and download input
cookies="$(cat ../cookies.txt)"
curl https://adventofcode.com/2022/day/$1/input --output input.txt --cookie "$cookies"

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
  sed -i "s/day_template/day_$1/g" CMakeLists.txt # Linux version
elif [[ "$OSTYPE" == "darwin"* ]]; then
  sed -i '' "s/day_template/day_$1/g" CMakeLists.txt # macos need '' argument
fi

touch input_example.txt

git add *
