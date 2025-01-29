# NFC Card Operations
`This file defines all the NFC card operations supported by X1 Card`

## Operations
- [1. Check Card Pairing](#1-check-card-pairing)
- [2. Card Pairing](#2-card-pairing)
- [3. Read Card Version](#3-read-card-version)
- [4. Fetch Card Share](#4-fetch-card-shares)
- [5. Write Card Shares](#5-write-card-shares)
- [6. Read Verify Card Shares](#6-read-verify-card-shares)
- [7. Delete Share](#7-delete-share)
- [8. Fetch Wallet List](#8-fetch-wallet-list)
- [9. Fetch Challenge](#9-fetch-challenge)
- [10. Sign Challenge](#10-sign-challenge)
- [11. Unlock Wallet](#11-unlock-wallet)
- [12. Encrypt Data](#12-encrypt-data)
- [13. Decrypt Data](#13-decrypt-data)


## 1. Check Card Pairing

This card operation handles card tap and pairing check. Selects the card and checks if the card is paired with the X1 Vault. Verifies the family id and the card id from the X1 Vault flash. 

### SELECT APPLET APDU Command-Response
Select command will be coded as the following table.
<table>
 <caption id="multi_row">Table 1:Select Applet Command APDU</caption>
<tr>
<th>
TAG
</th>
<th>
VALUE
</th>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;A4&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;04&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;08&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">DATA</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Applet AID</span></p>
</td>
</tr>
</table>

Applet will respond with the following table.

<table>
 <caption id="multi_row">Table 2: Select Applet Response APDU</caption>
<tr>
<th>
<p><span style="font-weight: 400;">TAG</span></p>
</th>
<th>
<p><span style="font-weight: 400;">Length</span></p>
</th>
<th>
<p><span style="font-weight: 400;">Value</span></p>
</th>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;B0&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;02&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Version</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;B1&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;05&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Family ID with last byte as Card Number</span></p>
</td>
</tr>
</table>


## 2. Card Pairing

This card operation pairs X1 Vault with an X1 card. A public-private key pair is generated. Public will be available in response to this command. To generate key-pair, asymmetric algo ECC will be used and the curve will be P-256.

### Generate KEY-PAIR APDU Command-Response
<table>
 <caption id="multi_row">Table 3: Generate Key-Pair Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C5&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
</table>

<br>

<table>
 <caption id="multi_row">Table 4: Generate Key-Pair Response APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Public key(&lsquo;0xEA&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xxxxx&rsquo;</span></p>
</td>
</tr>
</table>


## 3. Read Card Version

This card operation reads the card version from an X1 card irrespective of it's pairing status. 

### SELECT APPLET APDU Command-Response
[APDU format](#select-applet-apdu-command-response)


## 4. Fetch Card Shares

This card operation retrieves the wallet data from the card, and shares the retrieved wallet data. Search for the wallet by wallet name and if found then it will match the password, once password is matched it will send the share in response to this command.

### RETRIVE WALLET APDU Command-Response
<table>
<caption id="multi_row">Table 5: Retrieve wallet Command APDU</caption>
<tr>
<th>
<p><span style="font-weight: 400;">TAG</span></p>
</th>
<th colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</th>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C2&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td rowspan="2"><br />
<p><span style="font-weight: 400;">DATA</span></p>
<br /><br /></td>
<td>
<p><span style="font-weight: 400;">Wallet name(0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Wallet password(0xE1)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Hash of Password</span></p>
</td>
</tr>
</table>

<br>
<br>


<table>
  <caption id="multi_row">Table 6: Retrieve wallet Response APDU</caption>
<tr>
<th>
<p><span style="font-weight: 400;">Tag</span></p>
</th>
<th>
<p><span style="font-weight: 400;">Length</span></p>
</th>
<th>
<p><span style="font-weight: 400;">Value</span></p>
</th>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">X Coordinate (0xE2)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">X Coordinate of wallet</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Number of Mnemonics (0xE3)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">To be Added</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Total Number of Share (0xE4)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">To be Added</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Wallet Share (0xE5)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;64&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Share generated by SSS</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Structure Checksum(0xE6)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Metadata for Chacha Poly</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Minimum Number Of Shares(0xE7)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>&nbsp;</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Has User Set Password (0xE8)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">A boolean specifying has user set password or not</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Key for encrypting xPUB (0xE9)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Card will only store this key</span></p>
</td>
</tr>
</table>

<br>
<br>

<table><caption id="multi_row">Table 7: Retrieve wallet Status Word</caption>
<tr>
<td>
<p><span style="font-weight: 400;">SW</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Meaning</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;9000&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">OK</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A83&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet not found or Wallet has been deleted</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6CXX&rsquo; where XX is remaining tries</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Password</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6DXX&rsquo; where XX is level of block</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Card is blocked</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A80&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">'6982'</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Security Status Not Satisfied</span></p>
</td>
</tr>
</table>


## 5. Write Card Shares

This card operation writes the wallet share to an X1 card. Checks if space is available to store new wallet, if it is available then applet will store information mentioned in the data part of the request apdu. Also, ensures there is no wallet name conflict.

### ADD WALLET APDU Command-Response
<table>
<caption id="multi_row">Table 8: Add wallet Command APDU</caption>
<tr>
<th>
<p><span style="font-weight: 400;">TAG</span></p>
</th>
<th colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</th>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C1&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td rowspan="13">
<p><span style="font-weight: 400;">DATA</span></p>
</td>
<td>
<p><strong>TLV</strong></p>
</td>
<td>
<p><strong>Length</strong></p>
</td>
<td>
<p><strong>DATA</strong></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Name(0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Password Double Hash(0xE1)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Double Hash of password</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">X Coordinate (0xE2)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">X Coordinate of wallet</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Number of Mnemonics (0xE3)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">To be Added</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Total Number of Share (0xE4)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">To be Added</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Wallet Share with MAC(0xE5)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;64&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Share generated by SSS with MAC</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Structure Checksum(0xE6)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;4&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Metadata for Chacha Poly</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Minimum Number Of Shares(0xE7)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">To be Added</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Has User Set Password (0xE8)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">A boolean specifying has user set password or not</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Key for encrypting xPUB (0xE9)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Card will only store this key</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Beneficiary Key (0xEA)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">This is an AES key which will be used for beneficiary / inheritance service.</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">IV for Beneficiary Key (0xEB)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Initial Vector for AES</span></p>
</td>
</tr>
</table>

<br>
<br>

<table>
<caption id="multi_row">Table 9: Add Wallet Status Word</caption>
<tr>
<th>
<p><span style="font-weight: 400;">SW</span></p>
</th>
<th>
<p><span style="font-weight: 400;">Meaning</span></p>
</th>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;9000&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">OK</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A84&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Card full</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6986&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Command not allowed</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A80&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6982&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Security status not satisfied</span></p>
</td>
</tr>
</table>


## 6. Read Verify Card Shares

This card operation reads back and verifies wallet share from an X1 card. Search for the wallet by wallet name and if found then it will match the password, once password is matched it will send the share in response to this command.

### RETRIVE WALLET APDU Command-Response
[APDU format](#retrive-wallet-apdu-command-response)

## 7. Delete Share

This card operation deletes wallet share data from an X1 card and updates wallet data on flash accordingly. Checks if wallet name matched, then it will verify password, if matched, wallet will be deleted.

### DELETE WALLET APDU Command-Response
<table>
  <caption id="multi_row">Table 10: Delete wallet Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C3&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td rowspan="2">
<p><span style="font-weight: 400;">DATA</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name(0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Wallet password(0xE1)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Hash of Password (x2)</span></p>
</td>
</tr>
</table>
<p>&nbsp;</p>

<br>
<br>

<table>
  <caption id="multi_row">Table 11: Delete wallet Status Word</caption>
<tr>
<td>
<p><strong>SW</strong></p>
</td>
<td>
<p><strong>Meaning</strong></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;9000&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">OK</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A83&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet not found</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6CXX&rsquo; where XX is remaining tries</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Password</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6DXX&rsquo; where XX is level of block</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Card is blocked</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A80&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6982&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Security status not satisfied</span></p>
</td>
</tr>
</table>


## 8. Fetch Wallet List

This card operation extracts the wallet list from an X1 card. To retrieve all wallet names send the below mentioned APDU, the card will send names of wallet stored in that card. No verification by password is required.

### LIST WALLET NAMES APDU Command-Response

Note : The first byte will return the number of wallets.

<table>
  <caption id="multi_row">Table 12: List Wallet Name Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C4&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
</table>

<br>
<br>

<table>
  <caption id="multi_row">Table 13: List Wallet Name Response APDU</caption>
<tr>
<th>
<p><span style="font-weight: 400;">Tag</span></p>
</th>
<th>
<p><span style="font-weight: 400;">Length</span></p>
</th>
<th>
<p><span style="font-weight: 400;">Value</span></p>
</th>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Wallet Info (0xE8)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">To be Added</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Is Stored Data Arbitary (0xA1)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">0x01 if Arbitary Data, Set else 0x00 by Default</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Is Wallet Locked (0xD4)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Skip</span></p>
</td>
<td>
<p><span style="font-weight: 400;">0x01 if Wallet Locked else 0x00</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Wallet Name (0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Name of Wallet</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Wallet ID (0xEC)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Hash of master public key (at m/1000'/0'/3'/1)</span></p>
</td>
</tr>
</table>


## 9. Fetch Challenge

This card operation fetches wallet challenge data from an X1 card. When the card gets locked due to incorrect pin tries, this operation is used to get the challenge which is to be solved and unlock the card. The challenge corresponding to the wallet name provided is returned. Subsequent calls to this APDU will not change the challenge. The challenge changes only if it is solved.

### GET CHALLENGE APDU Command-Response
<table>
  <caption id="multi_row">Table 14: Get Challenge Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;CB&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">DATA</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name(0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name</span></p>
</td>
</tr>
</table>

<br>

<table>
  <caption id="multi_row">Table 15: Get Challenge Response APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Target (&lsquo;0xD2&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Card will generate a hash to verify. Hash should be less than this target</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Random Number (&lsquo;0xD1&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">This random number will be used to calculate hash. The hash is then verified by card.</span></p>
</td>
</tr>
</table>

<br>

<table>
  <caption id="multi_row">Table 16: Get Challenge Status Word</caption>
<tr>
<td>
<p><span style="font-weight: 400;">SW</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Meaning</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;9000&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">OK</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A83&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet not found</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A80&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6200&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet is in unlock state</span></p>
</td>
</tr>
</table>


## 10. Sign Challenge

This card operation signs authentication data with the provided configuration. This will sign the provided data with a private key generated in “Generate Key-Pair”. 

### SIGN CHALLENGE APDU Command-Response

<table>
 <caption id="multi_row">Table 17: Sign Challenge Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C6&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">xxxxx</span></p>
</td>
</tr>
</table>

<br>

<table>
<caption id="multi_row">Table 18: Sign Challenge Response APDU</caption>

<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Signed Data(&lsquo;0xEB&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xxxxx&rsquo;</span></p>
</td>
</tr>

</table>


## 11. Unlock Wallet

This card operation is used to unlock a wallet by verifying the user's password and updating the wallet's locked status. The APDU is sent to the card to unlock it. In this APDU a nonce is sent to the card. The card gets unlocked if the following conditions are met.

Random Number = The number sent by card during Get Challenge APDU Nonce = Any 32 byte number

Target = The number sent by card during Get Challenge APDU If the SHA256 of Random Number concatenated with Nonce is less than Target then the card will get unlocked.

If the card is locked then any APDU which requires a PIN will return 0x6Dxx where xx is the number of times the card has been locked since the last correct PIN was entered.

If the challenge is satisfied and the card gets unlocked it will then verify the PIN sent in this APDU.

If the card is already unlocked then this APDU can also be used to verify PIN.

### VERIFY CHALLENGE APDU Command-Response

<table>
  <caption id="multi_row">Table 19: Verify Challenge Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;CB&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;01&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">DATA</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name(0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name</span></p>
</td>
</tr>
<tr>
<td>&nbsp;</td>
<td>
<p><span style="font-weight: 400;">Nonce (0xD3)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Any number</span></p>
</td>
</tr>
<tr>
<td>&nbsp;</td>
<td>
<p><span style="font-weight: 400;">Password (0xE1)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">PIN of the wallet (or its double hash)</span></p>
</td>
</tr>
</table>

<br>
<br>

<table>
  <caption id="multi_row">Table 20: Verify Challenge Response APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Target (&lsquo;0xD2&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Card will generate a hash to verify. Hash should be less than this target</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Random Number (&lsquo;0xD1&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;32&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">This random number will be used to calculate hash. The hash is then verified by card.</span></p>
</td>
</tr>
</table>

<br>

<table>
  <caption id="multi_row">Table 21: Verify Challenge Status Word</caption>
<tr>
<td>
<p><span style="font-weight: 400;">SW</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Meaning</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;9000&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">OK</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A83&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet not found</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A80&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6Cxx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">If the card is unlocked and the PIN is wrong. &lsquo;xx&rsquo; is the number of tries left before the card gets locked.</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6Dxx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">If the card is locked. Where &lsquo;xx&rsquo; is the number of times the card has been locked since the last correct PIN was entered.</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A88&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Challenge verification failed. Card is still in locked state.</span></p>
</td>
</tr>
</table>


## 12. Encrypt Data

This card operation will encrypt data using the AES key and IV which was stored in the X1 card during addition of the wallet.

### ENCRYPT DATA APDU Command-Response

<table>
  <caption id="multi_row">Table 22: Encrypt Data Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C9&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;01&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td rowspan="2">
<p><span style="font-weight: 400;">DATA</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name(0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Plain Data(0xD5)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
</tr>
</table>

<br>
<br>

<table>
  <caption id="multi_row">Table 23: Encrypt Data Response APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Encrypted Data (AES) (&lsquo;0xD6&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xxxxx&rsquo;</span></p>
</td>
</tr>
</table>

<br>
<br>

<table>
  <caption id="multi_row">Table 24: Encrypt Data Status Words</caption>
<tr>
<td>
<p><span style="font-weight: 400;">SW</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Meaning</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;9000&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">OK</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A83&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet not found</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A80&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6982&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Security status not satisfied</span></p>
</td>
</tr>
</table>

## 13. Decrypt Data

This card operation will decrypt data using the AES key and IV which was stored in the X1 card during addition of the wallet.

### DECRYPT DATA APDU Command-Response

<table>
<caption id="multi_row">Table 25: Decrypt Data Command APDU</caption>
<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">Value</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">CLA</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">INS</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;C9&rsquo;&nbsp;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P1</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">P2</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;00&rsquo;</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">LC</span></p>
</td>
<td colspan="3">
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
</tr>
<tr>
<td rowspan="2">
<p><span style="font-weight: 400;">DATA</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name(0xE0)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;16&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet name</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Encrypted data(0xD6)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data to be decrypted</span></p>
</td>
</tr>
</table>

<br>
<br>
<table>
  <caption id="multi_row">Table 26: Decrypt Data Response APDU</caption>

<tr>
<td>
<p><span style="font-weight: 400;">TAG</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Length</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">Decrypted Data (AES) (&lsquo;0xD5&rsquo;)</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xx&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">&lsquo;xxxxx&rsquo;</span></p>
</td>
</tr>

</table>

<br>

<table>
 <caption id="multi_row">Table 27: Decrypt Data Status Word</caption>
<tr>
<td>
<p><span style="font-weight: 400;">SW</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Meaning</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;9000&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">OK</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A83&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wallet not found</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6A80&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Wrong Data</span></p>
</td>
</tr>
<tr>
<td>
<p><span style="font-weight: 400;">&lsquo;6982&rsquo;</span></p>
</td>
<td>
<p><span style="font-weight: 400;">Security status not satisfied</span></p>
</td>
</tr>

</table>
