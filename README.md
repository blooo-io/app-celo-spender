# ledger-app-celo

Celo wallet application framework for Ledger Blue, Nano S and Nano X

This follows the specification available in the doc/ folder

For more information about the parsing framework you can refer to <https://www.ledger.com/dynamic-secure-applications-with-bolos-and-ledger-blue-a-use-case-with-ethereum-and-the-dao>

## How to run on Ledger Nano S/S+

- Clone the repo
- Open the repo on VSCode
- Open Docker Desktop
- Close Ledger Live if it was open (Ledger live will hijack the connection to the Ledger if it is open)
- Install the VSCode extension 'Ledger Developer Tools'
- Connect and Unlock your Ledger device to the computer (sometimes a hub/adapter can prevent the connection between the computer and the ledger)
- Click on the Ledger symbol to see the extension's menu.
  The following steps all use the Ledger Developer Tools
- Select the option 'Update Container'
- Click on 'Select target' and select your ledger device
- Select the option 'Build'
- Select the option 'Load app on device'
- On the ledger device, approve/accept all requests
- You should be good to go! Enjoy!
