def file_to_hex(file_path, output_file):
    try:
        with open(file_path, 'rb') as file:
            content = file.read()
            hex_list = [f'0x{byte:02X}' for byte in content]
            hex_lines = [', '.join(hex_list[i:i+16]) for i in range(0, len(hex_list), 16)]
            hex_content = ',\n'.join(hex_lines)
        hex_content = 'const uint8_t FILEUP_HTML[] PROGMEM = {\n' + hex_content + '};'
        with open(output_file, 'w') as file:
            file.write(hex_content)
            
        print(f"Hexadecimal content saved to {output_file}")
    except FileNotFoundError:
        return "File not found."
    except Exception as e:
        return f"An error occurred: {e}"

# Example usage
file_path = 'E:/MPPT_CHAGER/MPPTV5/index.html'
output_file = 'E:/MPPT_CHAGER/MPPTV5/output_file.txt'
file_to_hex(file_path, output_file)
