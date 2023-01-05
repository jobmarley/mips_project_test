import os
import sys

def write_coe_file(filepath, content):
    with open(filepath, 'w') as f:
        f.write("memory_initialization_radix=16;\n")
        f.write("memory_initialization_vector=\n")
        f.write(',\n'.join(content))
        f.write(';')


def parse_int(s):
    if s.startswith('0x'):
        return int(s, 16)
    return int(s)

output_path = ''

def parse_options(args):
    remaining = []
    i = 0
    while i < len(args):
        if args[i] == '-o':
            if i + 1 >= len(args):
                print('convert_to_coe.py: Command line error CTC0001 : argument -o requires a filename')
                exit()
            global output_path
            output_path = args[i + 1]
            i = i + 1
        else:
            remaining.append(args[i])
        i = i + 1
    return remaining

def main():
    if len(sys.argv) < 2:
        print('convert_to_coe.py: Command line error CTC0001 : Wrong arguments. Correct syntax is convert_to_coe.py <filename1>=<ofs> <filename2>=<ofs> -o <outpath>')
        exit(1)
        
    filenames = parse_options(sys.argv[1:])
    result = []
    current_ofs = 0
    for s in filenames:
        
        parts = s.split('=')
        fn = parts[0]
        if not os.path.isfile(fn):
            print('convert_to_coe.py: Command line error CTC0001 : {} doesnt exist!'.format(fn))
            exit(1)

        if len(parts) < 2:
            print('convert_to_coe.py: Command line error CTC0001 : no offset for file {}, correct syntax is <filename>=<offset>'.format(fn))
            exit(1)
        else:
            ofs = parse_int(parts[1])
            
        if ofs % 4 > 0:
            print('convert_to_coe.py: Command line error CTC0001 : offset must be a multiple of 4')
            exit(1)
        if ofs < current_ofs:
            print('convert_to_coe.py: Command line error CTC0001 : offset must be bigger than current file size')
            exit(1)



        while current_ofs < ofs:
            result.append('00000000')
            current_ofs = current_ofs + 4

        with open(fn, 'rb') as f:
            content = f.read()
            if len(content) % 4 > 0:
                print('convert_to_coe.py: Command line error CTC0001 : file size must be a multiple of 4 ({})'.format(fn))
                exit(1)
            for i in range(0, len(content), 4):
                result.append('{0:08X}'.format(int.from_bytes(content[i:i+4], 'little')))
                current_ofs = current_ofs + 4
       
    global output_path
    if not output_path:
        print('convert_to_coe.py: Command line error CTC0001 : no output file specified, use -o <filename>')
        exit(1)

    write_coe_file('{}'.format(output_path), result)
    print('{} successfully merged'.format(output_path))
    exit(0)

if __name__ == "__main__": 
    main()