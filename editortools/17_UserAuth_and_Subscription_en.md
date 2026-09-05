# Account Sign-In and Subscription

The PluginAuth panel signs you in to a SuperStage account and shows your subscription summary and module entitlements.

**Sign-in goes through the browser**: your web account authorizes this device. No password is entered in the plugin, and there is no email verification code.

---

## 1. Opening the Panel

SuperStage toolbar menu → **PluginAuth**.

---

## 2. Signing In

When signed out, the panel shows the sign-in explanation, the **Terms of Service** and **Privacy Policy** consent checkbox with links, and the **Sign in with Browser** button.

The sign-in button stays disabled until consent is ticked. Clicking either link opens the page in your default browser.

### 2.1 The Flow

1. Tick the consent for the Terms of Service and Privacy Policy;
2. Press **Sign in with Browser**. The panel **opens your default browser** on the approval page automatically;
3. A short **code** appears on the panel in large type, and the prompt changes to "Approve the sign-in request in your browser to continue". **Check that the code on the browser page matches the one on the panel**, then approve;
4. Once approved, the plugin completes sign-in on its own and the panel switches to the signed-in state.

While waiting:

- If the browser was closed by accident, press **Reopen the approval page** on the panel;
- To abort, press the same button — while waiting it reads **Cancel**.

### 2.2 How Long You Have

The authorization request is valid for **600 seconds (10 minutes)**, and the plugin asks the server for the result every **5 seconds**. If it is not approved within that window the request expires; just start a new one.

---

## 3. When Signed In

The panel shows: nickname, email, **Refresh**, **Sign Out**, the subscription area, and the module entitlement area.

- **Refresh** re-requests user state, current subscription, and the entitlement token;
- **Sign Out** calls the sign-out endpoint and clears the local session and entitlement cache.

---

## 4. Status Messages

| Status | Meaning |
| --- | --- |
| Signed In | Signed in |
| Signed in (expires in Xs) | Signed in, with the access token's remaining seconds |
| Please Sign In | Not signed in |
| Signing in... | Sign-in in progress |
| Refreshing... | Refreshing user, subscription, and entitlement information |
| Sign-in Failed | Sign-in failed |
| Session Expired | The session expired, or the server returned an authentication error |

---

## 5. Subscription Information

Displayed from the fields the server returns:

- **Plan** — plan name or tier. With no active subscription it reads `Plan: None`;
- **Remaining** — days left;
- **Expires** — expiry date;
- Approaching expiry shows **Expiring soon. Renew in time**.

The **Plan Comparison >** link alongside opens the plan comparison page.

---

## 6. Module Entitlements

The panel requests an encrypted entitlement token, and after verifying and decrypting it shows the module entitlements on the account. An entitlement may come from a subscription plan or from a **Standalone Module Entitlement** purchased on its own.

This manual makes **no promise** that a given plan includes a given module. What you can actually use is determined by the account's real entitlements, the purchase page, and what the server returns.

---

## 7. Local Session

After a successful sign-in the access token and refresh token are stored locally. Later requests carry a Bearer token; when the access token nears expiry or the server returns 401, one refresh is attempted automatically. If the refresh fails, the local session is cleared and you sign in again.

---

## 8. Common Questions

**The browser did not open**
Press **Reopen the approval page**. If nothing happens, check the system's default browser setting.

**The code in the browser does not match the panel**
Do not approve. Cancel and start again — a mismatch means the request you are approving did not come from this device.

**Approved, but the panel did not react**
The plugin polls every 5 seconds; give it a moment. After 10 minutes the request expires — press **Sign in with Browser** again.

**Sign in with Browser is greyed out**
The Terms of Service and Privacy Policy consent has not been ticked.

**Subscription or entitlements are not updating**
Press **Refresh** to re-request the server data.

**It says Session Expired**
Sign in again.
