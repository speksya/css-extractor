# CSS Selector Extractor for React

A small utility that parses React components to extract and process CSS class selectors

## Installation

### Building from Source

```bash
git clone https://github.com/speksya/css-extractor.git
cd css-extractor
make
```

## Configuration

### Configuration File

The tool requires a `configuration.toml` file in the current directory with the following structure:

- **`[classnames]` (required)**: Defines how to detect regular CSS class names
  - `keys` (array of strings): Property names used for regular class names in components

### Configuration Example

```toml
[classnames]
keys = [ "cn", "className" ]
```
