#!/bin/bash
echo "Running IWAD patcher tests on Linux..."

# Function to rename file to uppercase
rename_to_uppercase() {
    local file
    file="$1"
    local uppercase_file
    uppercase_file=$(echo "$file" | tr '[:lower:]' '[:upper:]')
    if [ "$file" != "$uppercase_file" ]; then
        mv "$file" "$uppercase_file"
    fi
}

# Check for each required IWAD
required_iwads=("DOOM.WAD" "DOOM2.WAD" "PLUTONIA.WAD" "TNT.WAD" "HERETIC.WAD" "HEXEN.WAD" "HEXDD.WAD" "STRIFE1.WAD")
for wad in "${required_iwads[@]}"; do
    # Find file in a case-insensitive way
    found_file=$(find . -iname "$wad" -print -quit)
    if [ -z "$found_file" ]; then
        echo "Error: Required file $wad not found."
        exit 1
    else
        rename_to_uppercase "$found_file"
    fi
done
echo "All required files are present."
echo

# Variable to track if any test failed
all_tests_passed=true

# Function to run tests and check for errors
run_test() {
    if ! ./iwadpatcher "$@"
    then
        echo "Error running test for $3"
        all_tests_passed=false
    fi
}

# Run tests
run_test -ODOOM_11.WAD -VDOOM_11 DOOM.WAD
run_test -ODOOM_11_KEX.WAD -VDOOM_KEX DOOM_11.WAD
run_test -ODOOM_KEX_11.WAD -VDOOM_11 DOOM_11_KEX.WAD
run_test -ODOOM2_1666G.WAD -VDOOM2_1666G DOOM2.WAD
run_test -ODOOM2_1666G_KEX.WAD -VDOOM2_KEX DOOM2_1666G.WAD
run_test -ODOOM2_KEX_1666G.WAD -VDOOM2_1666G DOOM2_1666G_KEX.WAD
run_test -OPLUTONIA_19.WAD -VPLUTONIA_19 PLUTONIA.WAD
run_test -OPLUTONIA_19_KEX.WAD -VPLUTONIA_KEX PLUTONIA_19.WAD
run_test -OPLUTONIA_KEX_19.WAD -VPLUTONIA_19 PLUTONIA_19_KEX.WAD
run_test -OTNT_19.WAD -VTNT_19 TNT.WAD
run_test -OTNT_19_KEX.WAD -VTNT_KEX TNT_19.WAD
run_test -OTNT_KEX_19.WAD -VTNT_19 TNT_19_KEX.WAD
run_test -OHERETIC_10.WAD -VHERETIC_10 HERETIC.WAD
run_test -OHERETIC_10_13.WAD -VHERETIC_13 HERETIC_10.WAD
run_test -OHERETIC_13_10.WAD -VHERETIC_10 HERETIC_10_13.WAD
run_test -OHEXEN_10.WAD -VHEXEN_10 HEXEN.WAD
run_test -OHEXEN_10_11.WAD -VHEXEN_11 HEXEN_10.WAD
run_test -OHEXEN_11_10.WAD -VHEXEN_10 HEXEN_10_11.WAD
run_test -OHEXDD_10.WAD -VHEXDD_10 HEXDD.WAD
run_test -OHEXDD_10_11.WAD -VHEXDD_11 HEXDD_10.WAD
run_test -OHEXDD_11_10.WAD -VHEXDD_10 HEXDD_10_11.WAD
run_test -OSTRIFE1_10.WAD -VSTRIFE1_10 STRIFE1.WAD
run_test -OSTRIFE1_10_131.WAD -VSTRIFE1_131 STRIFE1_10.WAD
run_test -OSTRIFE1_131_10.WAD -VSTRIFE1_10 STRIFE1_10_131.WAD

# Cleanup WAD files
rm "DOOM_11.WAD" "DOOM_11_KEX.WAD" "DOOM_KEX_11.WAD" \
   "DOOM2_1666G.WAD" "DOOM2_1666G_KEX.WAD" "DOOM2_KEX_1666G.WAD" \
   "PLUTONIA_19.WAD" "PLUTONIA_19_KEX.WAD" "PLUTONIA_KEX_19.WAD" \
   "TNT_19.WAD" "TNT_19_KEX.WAD" "TNT_KEX_19.WAD" \
   "HERETIC_10.WAD" "HERETIC_10_13.WAD" "HERETIC_13_10.WAD" \
   "HEXEN_10.WAD" "HEXEN_10_11.WAD" "HEXEN_11_10.WAD" \
   "HEXDD_10.WAD" "HEXDD_10_11.WAD" "HEXDD_11_10.WAD" \
   "STRIFE1_10.WAD" "STRIFE1_10_131.WAD" "STRIFE1_131_10.WAD"

# Print result
echo
if $all_tests_passed; then
    echo "All tests passed successfully."
    exit 0
else
    echo "One or more tests failed."
    exit 1
fi
