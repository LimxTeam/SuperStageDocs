# SuperStage User Authentication & Subscription — User Manual

## 1. Overview

The User Authentication Panel (Plugin Auth) is used to manage your SuperStage account and subscription status. Some advanced features of SuperStage (such as DMX signal transmission, MA macro export, etc.) require a valid subscription to use. Through this panel, you can log in to your account using email verification codes, view subscription details, and refresh authorization status.

---

## 2. Access

**Main Menu Path**: Toolbar **SuperStage** dropdown menu → **PluginAuth**

A fixed-size standalone window (460 × 620 pixels) will pop up, with the title displaying `SuperStage {Version}`.

---

## 3. Interface Description

The authentication panel has two views — **Logged Out** and **Logged In** — which switch automatically based on the current authentication state.

### 3.1 Logged Out View

```
┌──────────────────────────────────────────┐
│  SuperStage v2.x.x          ● Status     │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ │
│                                          │
│  Email                                   │
│  ┌──────────────────────────────────┐   │
│  │ [your@email.com               ]  │   │
│  └──────────────────────────────────┘   │
│                                          │
│  Verification Code                       │
│  ┌──────────────────────────────────┐   │
│  │ [______]    [Send Code / 60s]    │   │
│  └──────────────────────────────────┘   │
│                                          │
│  No account yet? We'll create one for    │
│  you automatically.                      │
│                                          │
│              [Sign In]                   │
│                                          │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ │
│  SuperStage v2.x.x          yunsio.com   │
└──────────────────────────────────────────┘
```

### 3.2 Logged In View

```
┌──────────────────────────────────────────┐
│  SuperStage v2.x.x       ● Logged in    │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ │
│                                          │
│  [Refresh]                  [Sign Out]   │
│                                          │
│  ┌──────────────────────────────────┐   │
│  │ Username: designer              │   │
│  │ Nickname: LightDesigner         │   │
│  │ Email: designer@example.com     │   │
│  │ Registration Date: 2025-06-15   │   │
│  │                                  │   │
│  │ ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━  │   │
│  │ 【Subscription Status】          │   │
│  │  Personal annual                │   │
│  │                                  │   │
│  │ ✓ Personal Annual               │   │
│  │   Subscription Term:            │   │
│  │     2025-06-15 ~ 2026-06-15     │   │
│  │   Remaining: 102 days           │   │
│  └──────────────────────────────────┘   │
│                                          │
│  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ │
│  SuperStage v2.x.x          yunsio.com   │
└──────────────────────────────────────────┘
```

---

## 4. Feature Details

### 4.1 Status Indicator

A circular status indicator and text are located on the right side of the top brand bar:

| Status Text | Description |
|-------------|-------------|
| **Logged in** | Account is logged in |
| **Logged in (expires in Xs)** | Logged in; shows token countdown |
| **Refreshing...** | Refreshing user information |
| **Please login** | Not logged in |
| **Logging in...** | Logging in |
| **Login failed** | Login failed |
| **Session expired** | Session expired; needs re-login |

### 4.2 Email Verification Code Login

SuperStage uses **email verification code** login; no password setup is required.

| Step | Action |
|------|--------|
| 1 | Enter your email address |
| 2 | Click the **"Send Code"** button; the system sends a verification code to your email |
| 3 | Enter the received verification code |
| 4 | Click the **"Sign In"** button to complete login |

> **Auto-Registration**: If your email has not yet registered an account, the system will automatically create an account on your first login; no separate registration is needed.

#### Verification Code Cooldown

After clicking "Send Code", the button enters a **60-second cooldown countdown**, during which resending is disabled. You can send again after the countdown ends.

### 4.3 Action Buttons

| Button | Visible State | Function |
|--------|---------------|----------|
| **Send Code** | Logged out | Send a verification code to the entered email |
| **Sign In** | Logged out | Log in using email and verification code |
| **Refresh** | Logged in | Re-fetch the latest user and subscription information from the server |
| **Sign Out** | Logged in | Sign out of the currently logged-in account |

### 4.4 Account Info Area

After successful login, the central area of the panel displays merged user and subscription information:

**User Info Section**:

| Field | Description |
|-------|-------------|
| **Username** | User name |
| **Nickname** | Nickname |
| **Email** | Registered email |
| **Registration Date** | Registration date |

**Subscription Info Section** (below the separator):

| Field | Description |
|-------|-------------|
| **Subscription Status** | Current subscription type name |
| **Subscription Term** | Subscription start and end dates |
| **Remaining** | Remaining days |

---

## 5. Subscription Types

| Type Code | Display Name | Permission Tier |
|-----------|--------------|-----------------|
| **TRIAL_3DAY** | Trial | Tier2 |
| **PERSONAL_YEARLY** | Personal Annual | Tier2 |
| **PERSONAL_LIFETIME** | Personal Lifetime | Tier3 |
| **ENTERPRISE_YEARLY** | Enterprise Annual | Tier3 |
| **ENTERPRISE_LIFETIME** | Enterprise Lifetime | Tier3 |

### 5.1 Permission Tier Description

| Tier | Description |
|------|-------------|
| **Tier1** | No valid subscription; advanced features restricted |
| **Tier2** | Trial / Personal Annual; most advanced features available |
| **Tier3** | Personal Lifetime / Enterprise; all features fully available |

### 5.2 Feature Permission Matrix

| Feature | Tier1 (No Subscription) | Tier2 (Trial/Personal Annual) | Tier3 (Lifetime/Enterprise) |
|---------|------------------------|-------------------------------|-----------------------------|
| Fixture placement and layout | Yes | Yes | Yes |
| Fixture library editing | Yes | Yes | Yes |
| DMX signal reception | Yes | Yes | Yes |
| DMX signal transmission | No | Yes | Yes |
| MVR import | Yes | Yes | Yes |
| MA macro export | No | Yes | Yes |
| SuperData sync | No | Yes | Yes |
| NDI input | No | Yes | Yes |
| LDLink drone | No | Yes | Yes |

---

## 6. Workflow

### First-Time Use

1. Open the User Authentication Panel (SuperStage → PluginAuth)
2. Enter your email address
3. Click **"Send Code"** and check your email for the verification code
4. Enter the verification code and click **"Sign In"**
5. If the email is not registered, the system automatically creates an account and logs in
6. After successful login, user info and subscription status are displayed automatically

### Refreshing Subscription Status

1. Open the User Authentication Panel
2. Click the **"Refresh"** button
3. The system re-fetches the latest user and subscription information from the server

### Signing Out

1. Click the **"Sign Out"** button
2. The panel returns to the logged-out view
3. Locally cached authentication information is cleared

---

## 7. Offline and Caching

- After the first successful login, authentication status is cached locally
- During a valid subscription period, the cached status is automatically loaded when the editor starts
- Tokens have an expiration period; the panel's top status bar shows `expires in Xs` countdown
- After the token expires, you need to log in again with a network connection
- It's recommended to connect to the network periodically to maintain authorization status

---

## 8. Multi-Device and Kick-Off Mechanism

- Device identification uses the current computer name (`ComputerName`)
- When the same account logs in on another device, the current device receives a notification: **"Your account has logged in on another device, current session is offline"**
- After being kicked offline, you need to log in again

---

## 9. Notes

- SuperStage uses email verification code login; **no password setup or memorization is needed**
- New emails are automatically registered on first login; no website registration is required
- You need to log in again after changing computers
- When advanced features are restricted, check whether the subscription has expired or whether your subscription tier covers those features
- If you see a "Session expired" prompt, simply log in again

---

## 10. FAQ

| Issue | Solution |
|-------|----------|
| Cannot receive verification code | Check that the email address is correct; check the spam/junk folder |
| Login failed | Confirm the verification code hasn't expired or been entered incorrectly; confirm network connection is normal |
| Subscription status not updating | Click "Refresh" to manually refresh |
| Advanced features unavailable | Check whether the subscription has expired or whether your subscription type includes that feature |
| Kicked offline | The same account logged in on another device; log in again on the current device |
| Cannot use after changing computers | Simply log in again on the new computer |
