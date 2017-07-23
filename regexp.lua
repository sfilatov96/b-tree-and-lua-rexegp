--
-- Created by IntelliJ IDEA.
-- User: sfilatov96
-- Date: 23.07.17
-- Time: 16:30
-- To change this template use File | Settings | File Templates.
--

file = ""
regexp = ""

if arg[1] and arg[2] then
    file = arg[1]
    regexp = arg[2]
else
    print("Error: Unexpected Command variables!");
    os.exit()
end

for line in io.lines(file) do
    print(line)
end

