// SPDX-License-Identifier: Apache-2.0

const { spawnSync } = require('child_process');
const fs = require('fs');
const path = require('path');

const GithubBinaryHost = 'https://prebuilt.libkungfu.io';
const defined = (e) => e;

const utils = {
  exitOnError: function (error) {
    console.error(error);
    process.exit(-1);
  },

  getScope: function (npmConfigValue) {
    return npmConfigValue === 'undefined' ? '[package.json]' : '[user]';
  },

  trace: function (cmd, argv, opts) {
    if (!opts.silent) {
      console.log(`$ ${cmd} ${argv.join(' ')}`);
    }
  },

  getCoreGypDir: function () {
    const local = process.cwd().toString() === path.dirname(__dirname);
    return local ? '.gyp' : path.resolve(__dirname);
  },

  getNpmConfigValue: function (key) {
    return shell.runAndCollect(
      'npm',
      ['config', 'get', key, '--no-workspaces'],
      { silent: true },
    ).out;
  },

  findBinaryDependency: function (packageJson) {
    const hasBinary = (deps) =>
      Object.keys(deps)
        .map((key) => {
          const dependency = shell.getPackageJson(key);
          if (dependency && dependency.binary) {
            return dependency.name;
          }
        })
        .filter(defined);
    return [
      packageJson.dependencies || {},
      packageJson.optionalDependencies || {},
      packageJson.devDependencies || {},
    ]
      .filter(defined)
      .map(hasBinary)
      .flat();
  },

  setBinaryHostConfig: function (packageName) {
    const packageJson = shell.getPackageJson(packageName);
    if (!packageJson) {
      return;
    }
    if (packageJson.binary) {
      const key = `${packageJson.binary.module_name}_binary_host_mirror`;
      const binaryGithub = packageJson.binaryGithub;
      const override = binaryGithub && binaryGithub.host;
      const value = override ? binaryGithub.host : GithubBinaryHost;
      shell.npmCall(['config', 'set', key, value]);
    }
  },

  showProjectConfig: function (key) {
    const packageJson = shell.getPackageJson();
    const projectName = packageJson.name;
    const npmConfigKey = `${projectName}:${key}`;
    const npmConfigValue = utils.getNpmConfigValue(npmConfigKey);
    const value =
      npmConfigValue === 'undefined' ? packageJson.config[key] : npmConfigValue;
    console.log(
      `[config] ${npmConfigKey} = ${value} ${utils.getScope(npmConfigValue)}`,
    );
  },

  showBinaryHostConfig: function (packageName) {
    const packageJson = shell.getPackageJson(packageName);
    if (!packageJson) {
      return;
    }
    const key = packageJson.binary.module_name;
    const npmConfigKey = `${key}_binary_host_mirror`;
    const hostConfigValue = utils.getNpmConfigValue(npmConfigKey);
    const value =
      hostConfigValue === 'undefined' && packageJson
        ? packageJson.binary.host
        : hostConfigValue;
    console.log(
      `[binary] ${npmConfigKey} = ${value} ${utils.getScope(hostConfigValue)}`,
    );
  },
};

const shell = {
  getElectronArch: function () {
    try {
      const electron = require('electron');
      const electronVersionScript = path.resolve(
        path.dirname(__dirname),
        '.gyp',
        'electron-version.js',
      );
      const isLinux = process.platform === 'linux';
      const electronArgs = isLinux ? ['--no-sandbox'] : [];
      const result = shell.runAndCollect(
        electron,
        [...electronArgs, electronVersionScript],
        {
          silent: true,
        },
      ).out;
      const resolveHeadless = (arch) => {
        return arch || shell.getTargetArch();
      };
      return isLinux ? resolveHeadless(result) : result;
    } catch (e) {
      console.log(e);
      return undefined;
    }
  },

  getTargetArch: function () {
    return shell.getPackageJson('@kungfu-trader/kungfu-core').config.arch;
  },

  getPackageJson: function (packageName) {
    const toJSON = (filepath) => {
      return JSON.parse(fs.readFileSync(filepath, 'utf8').toString());
    };
    if (!packageName) {
      return toJSON(path.resolve(process.cwd().toString(), 'package.json'));
    }
    try {
      return toJSON(require.resolve(`${packageName}/package.json`));
    } catch (e) {
      return undefined;
    }
  },

  getConfigValue: function (name) {
    return process.env[`npm_package_config_${name}`];
  },

  npmCall: function (npmArgs) {
    return shell.run('npm', npmArgs);
  },

  verifyElectron: function () {
    const electronArch = shell.getElectronArch();
    const targetArch = shell.getTargetArch();
    if (electronArch === targetArch) {
      return;
    }
    console.error(
      `Electron arch ${electronArch} does not match target ${targetArch}`,
    );
    console.log(`Try to fix it by reinstall electron [${targetArch}]`);
    try {
      const electronModulePath = path.dirname(
        require.resolve('electron/package.json'),
      );
      const yarnIntegrityPath = path.join(
        path.dirname(electronModulePath),
        '.yarn-integrity',
      );
      fs.rmSync(electronModulePath, {
        force: true,
        recursive: true,
      });
      fs.rmSync(yarnIntegrityPath, { force: true });
    } catch (e) {
      console.warn('Failed to remove electron module');
    }
    shell.run(
      'yarn',
      ['install', '--frozen-lockfile', '--network-timeout=1000000', '--silent'],
      true,
      {
        silent: true,
        env: {
          ...process.env,
          npm_config_arch: targetArch,
        },
      },
    );
    const reinstalledElectronArch = shell.getElectronArch();
    console.log(`Reinstall electron [${reinstalledElectronArch}] done`);
    if (reinstalledElectronArch !== targetArch) {
      console.error(
        `Reinstall electron [${reinstalledElectronArch}] failed to match [${targetArch}]`,
      );
      console.error(
        `Please fix it manually by yarn install with environment variable npm_config_arch set to ${targetArch}`,
      );
      process.exit(-1);
    }
  },

  run: function (cmd, argv = [], check = true, opts = {}) {
    const real_cwd = fs.realpathSync(path.resolve(process.cwd().toString()));
    utils.trace(cmd, argv, opts);
    const result = spawnSync(cmd, argv, {
      shell: true,
      stdio: 'inherit',
      windowsHide: true,
      cwd: real_cwd,
      ...opts,
    });
    if (check && result.status !== 0) {
      process.exit(opts.tolerant ? 0 : result.status);
    }
    return result;
  },

  runAndCollect: function (cmd, argv = [], opts = {}) {
    utils.trace(cmd, argv, opts);
    const result = spawnSync(cmd, argv, {
      shell: true,
      stdio: 'pipe',
      windowsHide: true,
      ...opts,
    });
    result.out = result.stdout.toString().trim();
    return result;
  },

  runAndExit: function (cmd, argv = [], opts = {}) {
    const result = shell.run(cmd, argv, false, opts);
    if (result.status === 0) {
      process.exit(0);
    }
    return result;
  },

  setAutoConfig: function () {
    const isGithubEnv = () => process.env.CI && process.env.GITHUB_ACTIONS;
    if (!isGithubEnv()) {
      return;
    }
    const packageJson = shell.getPackageJson();
    if (packageJson.configGithub) {
      Object.keys(packageJson.configGithub).forEach((key) => {
        shell.npmCall([
          'config',
          'set',
          `${packageJson.name}:${key}`,
          packageJson.configGithub[key],
        ]);
      });
    }
    if (packageJson.binary) {
      utils.setBinaryHostConfig();
    }
    utils.findBinaryDependency(packageJson).map(utils.setBinaryHostConfig);
  },

  showAutoConfig: function () {
    const packageJson = shell.getPackageJson();
    if (packageJson.config) {
      Object.keys(packageJson.config).map(utils.showProjectConfig);
    }
    if (packageJson.binary) {
      utils.showBinaryHostConfig();
    }
    utils.findBinaryDependency(packageJson).map(utils.showBinaryHostConfig);
  },

  touch: function (filename, dirname = process.cwd().toString()) {
    const now = new Date();
    const filepath = path.resolve(dirname, filename);
    fs.utimesSync(filepath, now, now);
  },

  utils: utils,
};

module.exports = shell;
