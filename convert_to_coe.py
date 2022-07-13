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
def main():
    if len(sys.argv) < 2:
        print('Wrong arguments. Correct syntax is convert_to_coe.py <filename1> <filename2>')
        exit()
    filenames = sys.argv[1:]
    result = []
    current_ofs = 0
    for fn in filenames:
        
        if not os.path.isfile(fn):
            print('{} doesnt exist!'.format(fn))
            exit()

        ofs = parse_int(input('enter offset for file {}\n'.format(fn)))
        if ofs % 4 > 0:
            print('offset must be a multiple of 4')
            exit()
        if ofs < current_ofs:
            print('offset must be bigger than current file size')
            exit()

        while current_ofs < ofs:
            result.append('00000000')
            current_ofs = current_ofs + 4

        with open(fn, 'rb') as f:
            content = f.read()
            if len(content) % 4 > 0:
                print('file size must be a multiple of 4 ({})'.format(fn))
                exit()
            for i in range(0, len(content), 4):
                result.append('{0:08X}'.format(int.from_bytes(content[i:i+4], 'little')))
                current_ofs = current_ofs + 4
                
    out_filename = input('enter output filename:\n')
    write_coe_file('{}'.format(out_filename), result)
    print('DONE')

if __name__ == "__main__": 
    main()