@echo off
echo =========================================================================
echo LIVE GIT STASH DEMONSTRATION
echo =========================================================================
echo Scenario: You are editing checkout_load_test.jmx, but an urgent hotfix is needed.
echo.
echo Step 1: Simulating an in-progress modification...
echo <!-- WIP: Temporary debug assertion added --> >> scripts/checkout_load_test.jmx
echo Status before stashing:
git status -s
echo.
echo Press any key to STASH this work and clean the desk...
pause >nul
echo.
echo Running: git stash push -m "WIP: debug assertion on payment"
git stash push -m "WIP: debug assertion on payment"
echo.
echo Status after stashing (Desk is clean! Ready to switch branches):
git status -s
echo.
echo Inspecting stash list:
git stash list
echo.
echo Press any key to POP the stash and resume work...
pause >nul
echo.
echo Running: git stash pop
git stash pop
echo.
echo Status after popping stash:
git status -s
echo.
echo Cleaning up temporary demo change:
git restore scripts/checkout_load_test.jmx
echo Done! Working directory restored to clean state.
pause

