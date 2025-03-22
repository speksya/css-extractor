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

- **`[configuration]` (required)**: Contains global settings for the tool
  - `cssModulesEnabled` (boolean): Enables/disables CSS modules support

- **`[classnames]` (required)**: Defines how to detect regular CSS class names
  - `keys` (array of strings): Property names used for regular class names in components

- **`[cssmodules]` (conditionally required)**: Defines how to detect CSS module references
  - Required only when `cssModulesEnabled = true`
  - `keys` (array of strings): Variable names used for imported CSS modules

### Configuration Examples

#### Minimal Configuration (CSS Modules Disabled)

```toml
[configuration]
cssModulesEnabled = false

[classnames]
keys = [ "className" ]
```

#### Full Configuration with CSS Modules

```toml
[configuration]
cssModulesEnabled = true

[classnames]
keys = [ "c", "class", "className" ]

[cssmodules]
keys = [ "s", "style", "styles" ]
```
