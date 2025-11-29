Import("env")
import os

def load_env_file(filepath='.env'):
    """Load environment variables from .env file"""
    env_vars = {}
    
    if not os.path.exists(filepath):
        print(f"Warning: {filepath} not found. Using default values.")
        return env_vars
    
    with open(filepath, 'r') as f:
        for line in f:
            line = line.strip()
            # Skip empty lines and comments
            if not line or line.startswith('#'):
                continue
            
            # Parse KEY=VALUE
            if '=' in line:
                key, value = line.split('=', 1)
                key = key.strip()
                value = value.strip()
                
                # Remove quotes if present
                if value.startswith('"') and value.endswith('"'):
                    value = value[1:-1]
                elif value.startswith("'") and value.endswith("'"):
                    value = value[1:-1]
                
                env_vars[key] = value
    
    return env_vars

# Load environment variables from .env
env_vars = load_env_file('.env')

# Define variables to pass to build
required_vars = [
    'WIFI_SSID',
    'WIFI_PASSWORD',
    'HUE_BRIDGE_IP',
    'HUE_USERNAME',
]

# Build flags
build_flags = []
for key in required_vars:
    value = env_vars.get(key, '')
    if value:
        # Escape quotes and backslashes in the value
        escaped_value = value.replace('\\', '\\\\').replace('"', '\\"')
        build_flags.append(f'-D{key}=\\"{escaped_value}\\"')
        print(f"✓ Loaded {key}")
    else:
        print(f"⚠ Warning: {key} not found in .env file")

# Add flags to environment
env.Append(CPPDEFINES=build_flags)

print("\n✓ Environment variables loaded from .env")