cd "$(dirname "$0")"

rm -rf Updated_Config Current_Config

mkdir Updated_Config Current_Config

cp ../../Marlin/Configuration* Current_Config/

CONFIG_VERSION=$(grep -E "CONFIGURATION_.*_VERSION" Current_Config/Configuration.h | xargs | tr -d ' ')

if [ ! -d "Configurations" ]; then
git clone https://github.com/MarlinFirmware/Configurations.git
fi

cd Configurations

git fetch origin
git checkout -f origin/import-2.0.x
git branch -D autoupdater
git reset --hard

BASE_DIR=config/default

I=0
J=0

declare -a COMMITS=($(git log --oneline $BASE_DIR/Configuration.h | cut -d' ' -f1))

for commit in "${COMMITS[@]}"
do
I=$((I+1))
VERSION=$(git show $commit:$BASE_DIR/Configuration.h | grep -E "CONFIGURATION_.*_VERSION" | xargs | tr -d ' ')
if [ $CONFIG_VERSION = $VERSION ]; then
J=$I
elif [ $J -ne 0 ]; then
break
fi
done

BASE_COMMIT=${COMMITS[$J]}

git checkout -f $BASE_COMMIT

git checkout -b autoupdater

cp ../Current_Config/* $BASE_DIR/

git add .

git commit -m "user changes"

USER_COMMIT=$(git log --oneline | head -1 | cut -d' ' -f1)

git rebase origin/import-2.0.x

cp $BASE_DIR/* ../Updated_Config/

git rebase --abort

git checkout -f origin/import-2.0.x

cd ..

Green='\033[0;32m'
NC='\033[0m' # No Color
printf "${Green}Upgraded $(grep -E "CONFIGURATION_.*_VERSION" Current_Config/Configuration* | grep -oE "[0-9]+$" | sort | uniq) to $(grep -E "CONFIGURATION_.*_VERSION" Updated_Config/Configuration* | grep -oE "[0-9]+$" | sort | uniq)\n${NC}"

cp Updated_Config/Configuration* ../../Marlin/
