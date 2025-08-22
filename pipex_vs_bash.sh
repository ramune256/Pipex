#!/bin/sh

# pipex vs bash compatibility test
# Compares:
#   ./pipex file1 "cmd1" "cmd2" file2
# vs
#   < file1 cmd1 | cmd2 > file2

set -eu

ROOT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
PIPEX="$ROOT_DIR/pipex"
WORK_DIR="${ROOT_DIR}/.test_tmp"

mkdir -p "$WORK_DIR"

pass=0
fail=0

log() { printf "%s\n" "$*"; }

run_case() {
  case_name="$1"; shift
  infile="$1"; shift
  cmd1="$1"; shift
  cmd2="$1"; shift

  out_p="$WORK_DIR/${case_name}.pipex.out"
  out_b="$WORK_DIR/${case_name}.bash.out"
  err_p="$WORK_DIR/${case_name}.pipex.err"
  err_b="$WORK_DIR/${case_name}.bash.err"

  rm -f "$out_p" "$out_b" "$err_p" "$err_b"

  # pipex run
  ("$PIPEX" "$infile" "$cmd1" "$cmd2" "$out_p") 2>"$err_p"
  status_p=$?

  # bash pipeline run; quote-safe via bash -c with args
  bash -c '{ eval "< \"$1\" $2 | $3 > \"$4\""; } 2>"$5"' _ \
    "$infile" "$cmd1" "$cmd2" "$out_b" "$err_b"
  status_b=$?

  diff_out="$(diff -u -- "$out_b" "$out_p" 2>&1 || true)"
  diff_err="$(diff -u -- "$err_b" "$err_p" 2>&1 || true)"

  if [ "$status_p" -eq "$status_b" ] && [ -z "$diff_out" ] && [ -z "$diff_err" ]; then
    log "[PASS] $case_name"
    pass=$((pass + 1))
  else
    log "[FAIL] $case_name"
    log "  exit: pipex=$status_p bash=$status_b"
    if [ -n "$diff_out" ]; then
      log "  diff(out):\n$diff_out"
    fi
    if [ -n "$diff_err" ]; then
      log "  diff(err):\n$diff_err"
    fi
    fail=$((fail + 1))
  fi
}

# Prepare inputs
small_in="$WORK_DIR/small.txt"
big_in="$WORK_DIR/big.txt"
printf 'hello\nworld\nfoo bar\n' > "$small_in"
yes "lorem ipsum dolor sit amet" | head -n 50000 > "$big_in"

# Prepare a non-executable file
nonexec="$WORK_DIR/nonexec.sh"
printf '#!/bin/sh\necho hi\n' > "$nonexec"
chmod 644 "$nonexec"

# Cases
run_case "simple_cat_wc" "$small_in" "cat" "wc -l"
run_case "grep_quote_wc" "$small_in" "grep 'foo bar'" "wc -c"
run_case "abs_ok_cat_wc" "$small_in" "/bin/cat" "wc -l"
run_case "no_such_abs" "$small_in" "/no/such/cmd" "wc -l"
run_case "is_directory" "$small_in" "/" "wc -l"
run_case "perm_denied" "$small_in" "$nonexec" "wc -l"
run_case "big_file" "$big_in" "cat" "wc -l"

log ""
log "Summary: pass=$pass, fail=$fail"
[ "$fail" -eq 0 ]
