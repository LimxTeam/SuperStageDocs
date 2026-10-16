# SuperStage Privacy Policy

**Version: 4.4**
**Publication Date: October 1, 2026**
**Effective Date: October 1, 2026; for users who first register, purchase, activate, or expressly agree to this Policy after the Effective Date, this Policy takes effect on the date of their express agreement**

This Privacy Policy explains how Foshan Yierranran Technology Co., Ltd. ("**we**", "**us**", the "**Company**") collects, uses, stores, shares, and protects personal information when you use the SuperStage products and related services. It also explains the choices and rights available to you.

This English version is written for our international users and addresses, in addition to the laws of the People's Republic of China ("**PRC**") that primarily govern us, the requirements of other privacy frameworks that may apply to you — including the EU/UK General Data Protection Regulation ("**GDPR**") and comparable laws. Where local mandatory law grants you greater rights than this Policy, that law prevails for you.

> **Key points at a glance:**
>
> 1. **We are a company established in China, and our primary servers are located in mainland China** (Tencent Cloud, Guangzhou region). Your account and license data is processed there, with supporting services from Cloudflare (global network and daily off-site database backups containing the same account data), Google (optional sign-in), Stripe (card payments and related invoices), Tencent Exmail (email), and Tencent WeCom (internal payment notifications). See Section 6 and Appendix A for locations and purposes.
> 2. **Sign-in is passwordless.** We never ask you to create or store a password. Login uses one-time email codes or your Google account.
> 3. **The SuperStage plugin contains no telemetry.** It does not upload usage analytics, behavioral data, crash dumps, or any of your project content. Its only network traffic is license sign-in, session validation, sign-out, and an anonymous update check.
> 4. **Your creative work stays on your machine.** Unreal Engine projects, lighting designs, patch data, MVR/GDTF files, DMX network data, and NDI streams are processed locally and are never uploaded to us unless you actively send them to us (e.g., files emailed to support, or a link you submit to the showcase).
> 5. We use **no third-party analytics, advertising, or tracking tools** — only a minimal first-party page-view counter.
> 6. If you set up accounts or seats for others (employees, team members, students), you must have a lawful basis to share their information with us and must inform them of this Policy.

---

## 1. Who We Are and What This Policy Covers

### 1.1 Controller

- **Company:** Foshan Yierranran Technology Co., Ltd.
- **Unified Social Credit Code (China):** 91440606MACXPYQA5R
- **Established:** September 15, 2023
- **Registered address:** Nanhai District, Foshan, Guangdong Province, China
- **Website:** https://yunsio.com
- **Privacy contact:** yunsio@yunsio.com

For the processing described in this Policy, the Company is the data controller (in PIPL terms, the "personal information processor"). Third parties such as your bank, Google, or your email provider decide their own processing purposes and are independent controllers; please review their privacy notices.

### 1.2 Services covered

- The yunsio.com website and its subdomains (www.yunsio.com, edu.yunsio.com);
- The SuperStage Unreal Engine plugin and its built-in modules (including SuperLaser, SuperNdi, SuperMadrix, and the free SuperShader module);
- SuperStage accounts, sign-in, license activation, device management, orders, downloads, and after-sales support;
- Technical support, showcase ("Featured Works") submissions, and business communications through official channels.

This Policy does not cover third-party products or services (Unreal Engine itself, grandMA software, NDI tools, your bank, etc.).

## 2. Personal Data We Collect

### 2.1 Account and sign-in (passwordless)

Our accounts have no passwords. Registration and sign-in use one-time email verification codes or Google Sign-In.

| Category | Data | Purpose | Required? |
|---|---|---|---|
| Account | Email address, account ID, account status, registration region, language preference, time zone | Creating and managing your account, providing licenses and support | Yes |
| Profile | Display name, avatar (URL), country/region, company name, job title, website | Optional profile details | No — you choose what to fill in |
| Email verification | Code send requests, verification results, timestamps, failure counts (the code itself is stored only as a short-lived hash and expires after use) | Sign-in verification, preventing account takeover and abuse | Yes |
| Google Sign-In | Google account identifier (sub), email, display name, avatar URL, and language, from the openid/email/profile scopes you authorize | Creating or signing in to your account with Google | Only if you choose Google Sign-In |
| Agreement records | Version of the terms/policy agreed to, timestamp, account, IP address | Evidence of contract formation, dispute handling | Yes |
| Organization data | Organization name and type, member relationships, seat roles; for education institutions: institution profile, instructor bindings, and student enrollments | Team / education account management | Only for organization features |

### 2.2 Licensing, device activation, and anti-abuse

| Category | Data | Purpose | Required? |
|---|---|---|---|
| License data | License number, plan (edition), term, status, seats, module entitlements, activation and expiry times | License delivery, entitlement verification, device management | Yes |
| Device fingerprint | A SHA-256 **hash** of a device identifier, plus a fingerprint version tag. **The hash is computed locally by the plugin (currently derived from the Windows MachineGuid combined with a fixed salt); the raw hardware identifier never leaves your device, and our server rejects anything that is not a hash** | Recognizing authorized devices, enforcing device limits, preventing piracy and trial abuse | Yes |
| Device context | Device name (computer name), OS type and version, plugin version, time zone, OS region and locale settings | Compatibility, license verification, anomaly detection; also shown back to you on the device-authorization page so you can recognize your own device | Yes |
| Sign-in and sessions | IP address, sign-in time, session identifiers, validation results and failure reasons, User-Agent | Maintaining sessions, security verification, anomaly detection | Yes |
| Risk records | Trial history (the first plugin sign-in record is retained permanently), device–account association records, temporary suspension records and their end dates | Enforcing "one trial account per device" and similar anti-abuse rules; handling appeals | Within necessary scope |

**Plugin network transparency:** the plugin communicates only with api.yunsio.com, and only for: device-authorization sign-in (sending the device context above), session heartbeat (sending the session token only), sign-out, and an anonymous version check. **It does not upload usage statistics, editor behavior, frame rates, crash reports, or your project content.** The sign-in token is stored on your machine encrypted with the operating system's data-protection API (Windows DPAPI) and contains no plaintext personal data.

If we separately agree to provide you an **offline-licensed build**: the plugin generates a machine code locally (a hash derived from system identifiers — the raw identifiers never leave your device); you send us that code and receive an activation code bound to that machine. The offline build performs no network communication at all, and activation data is stored locally in encrypted form.

### 2.3 Orders, payment, refunds, and invoices

**For new online orders, card payments in EUR are handled by Stripe, and CNY scan-to-pay payments by WeChat Pay (Tenpay).** Enterprise purchases and staff-created orders may still be paid by bank transfer or another agreed offline method, verified manually. Historical orders keep their original currency and payment-channel records. The card-entry component or WeChat app handles payment-account credentials directly; we separately process the billing details submitted at checkout and the resulting transaction information, as described below.

| Category | Data | Purpose |
|---|---|---|
| Order data | Order number, product or plan, SKU, term, quantity, amount and actual currency (including CNY and EUR), order time, status; protected renewal-price records for qualifying orders | Processing purchases, delivery, protected renewals, and after-sales service |
| Billing details | Purchaser name (personal or company name), billing email, country, street address and additional address line, city, state or region, postal code; required fields depend on the checkout form and country | Identifying the purchaser, linking the payment customer, card processing, and invoicing; saved with the order for verification and later checkout prefill |
| Payment confirmation and customer linkage | Payment method, status, confirmation time, customer and payment identifiers and transaction references returned by the payment institution, refund records and amounts; links to our account or organization and order | Verifying payment, reconciliation, refunds, and avoiding incorrect customer associations |
| Payment materials | Bank receipts, transfer screenshots, remitter name — only what you actively submit | Manual verification of transfers and unusual orders |
| Invoice data | Billing details and, where needed for the requested invoice, contact name, tax ID, address or phone, bank details, delivery information; invoice number, status, currency, amount, hosted-invoice and PDF links, related credit notes | Verifying, issuing, or correcting documents, providing account access, and keeping statutory financial records |

Full card numbers, expiry dates, security codes, and WeChat Pay account credentials are collected and processed directly by the payment institutions (Stripe, WeChat Pay) or your bank and **never pass through or rest on our servers**. This does not mean that we do not process billing details. **For card payments, we send the necessary purchaser name, email, billing country and address, and account or organization and order references to Stripe to create or update the corresponding payment customer, process payment, reconcile records, and handle invoices. We receive payment, refund, and invoice status, related identifiers, and document links.** Processing may take place internationally; see Section 6 and Appendix A.5. Saving billing information does not mean an invoice has been issued; issuance and account access are explained in Section 10.4 of the Terms of Service. Please redact unrelated information before sending receipts or screenshots.

After successful payment, we may use Tencent WeCom to notify authorized operations staff internally about the subscription. The message contains the username or display name, account email, billing country if provided, purchased plan and term, amount received and currency, a link to the related order, and daily and monthly aggregate receipts, to support fulfillment and reconciliation. **It does not include a full billing address, card details, or project content**, and is not published to other customers. See Appendix A.9.

### 2.4 Website visits, downloads, and security logs

When you visit the website, call our APIs, or download software, we — and Cloudflare, which provides our network and security services — may process:

- IP address, access time, time zone;
- Browser type (User-Agent), operating system, and device type;
- Requested pages, endpoints, file versions, download paths, referrer, and response status;
- Session identifiers, necessary cookies, CSRF tokens;
- Request rates, error records, security challenges, and access-control logs;
- Download token status (single-use tokens valid for 15 minutes).

**Default pricing currency:** we use country or region information inferred from your visiting IP address, supplied by the network entry point / Cloudflare, to choose an initial price currency. Without a saved preference, mainland China defaults to CNY; other or unidentified locations default to EUR. This may be affected by proxy networks. It uses neither GPS nor precise location, and does not establish your actual billing address or tax status. You can switch manually; your browser saves the selected currency separately from your interface language.

**Site analytics are first-party only:** we record the page path, referrer, a random visitor identifier generated in your browser (ss_vid), IP address, and User-Agent to count visits and unique visitors. We use no third-party analytics, advertising, or behavioral profiling tools (see Section 4).

For plugin users, we additionally keep a per-account, per-day activity aggregate (daily heartbeat count and last device) to measure active-user counts. We do not record what you do inside the software.

### 2.5 Support, feedback, showcase submissions, and business communications

When you contact us or use these features, we may process:

- Support tickets: subject, category (technical/license/billing/download), priority, and description (sign-in required) — tickets do not accept file uploads; email us if you need to send a file;
- Feedback: type, rating, and comments;
- Showcase ("Featured Works") submissions: the video link (Bilibili or YouTube), description, and credit you email us — **approved submissions may be displayed publicly on our website**, shown as an embedded player from the platform you chose, which hosts the video file itself; nothing is stored on our servers. The license you grant us is described in the Terms of Service;
- Email correspondence: your email address, name, message body, and attachments;
- Environment details you choose to share (SuperStage version, Unreal Engine version, OS, hardware).

**Please do not include unrelated project files, client-confidential material, identity documents, full card numbers, credentials, or third-party protected content in support materials.** Redact anything unnecessary before submitting.

### 2.6 What we do NOT collect

To avoid any doubt, the current products and services do **not** collect:

- Passwords (no password system exists);
- Phone numbers (not collected for personal accounts; business contract contacts excepted);
- Biometric data (no face, fingerprint, or voice data);
- Precise geolocation (we process only IP addresses and your device's own time-zone/region settings);
- Raw hardware identifiers (MAC addresses, disk serials, CPU serials are never uploaded; the device fingerprint is a locally computed hash);
- In-app behavioral telemetry (keystrokes, feature usage, browsing);
- Automatic crash dumps (crash information reaches us only if you submit it with a support request).

If a future version introduces any such processing, we will update this Policy first and obtain consent where required.

### 2.7 Data that stays local

The following is processed on your device or local network and is not uploaded through sign-in or license verification:

- Unreal Engine projects, levels, blueprints, scenes, and configuration;
- Fixture patch data, Fixture IDs, universes, addresses, cues, and other lighting design data;
- The full contents of MVR, GDTF, grandMA2, and grandMA3 files (imports/exports are parsed locally);
- Art-Net and sACN DMX network data;
- NDI video streams, project media, textures, camera images, and render output;
- Models, materials, and fixture libraries you create or import;
- Local project files, caches, and editor logs.

Such content may be transmitted only if you actively submit it (support/showcase), if you configure third-party services (cloud drives, version control, remote desktop), or where strictly necessary to investigate a security incident or comply with a legal obligation.

### 2.8 Data received from third parties

- Google: the profile fields listed in 2.1 when you choose Google Sign-In;
- Organization or institution administrators: member emails and enrollment details they register (they are responsible for having informed the individuals and obtained any required consent);
- Courts, regulators, or authorities acting within their legal powers.

---

## 3. Purposes and Legal Bases

Where the GDPR or a similar law applies to you, we rely on the following legal bases:

| Purpose | Legal basis (GDPR) |
|---|---|
| Creating your account, sign-in, providing licenses, downloads, orders, and support | Performance of a contract (Art. 6(1)(b)) |
| Device activation, device limits, seat management | Performance of a contract (Art. 6(1)(b)) |
| Preventing attacks, fraud, piracy, trial abuse, and account takeover | Legitimate interests (Art. 6(1)(f)) — protecting our services and users; and legal obligations where applicable |
| Financial records, tax, and invoicing | Legal obligation (Art. 6(1)(c)) |
| Responding to your requests | Performance of a contract / steps at your request (Art. 6(1)(b)) |
| First-party site analytics | Legitimate interests (Art. 6(1)(f)) — minimal, first-party-only measurement |
| Public display of showcase submissions | Consent (Art. 6(1)(a)) — given when you submit |
| Marketing emails (if ever introduced) | Consent (Art. 6(1)(a)), withdrawable at any time |

Where PRC law applies, the corresponding bases under the Personal Information Protection Law (PIPL) apply, including necessity for contract performance, legal obligations, and separate consent for cross-border transfers where required. We do not sell personal information and do not use it for third-party targeted advertising.

---

## 4. Cookies and Local Storage

### 4.1 What we actually set

| Name | Type | Purpose | Lifetime |
|---|---|---|---|
| yunsio_session | Cookie (HttpOnly, Secure) | Sign-in session | ~12 hours |
| XSRF-TOKEN | Cookie | Cross-site request forgery protection | Session |
| yunsio_oauth_state | Cookie (HttpOnly) | Anti-forgery state during Google Sign-In | 10 minutes |
| web.locale | Cookie + localStorage | Remembering your language choice | 1 year |
| web.currency | localStorage | Remembering your selected pricing / checkout currency (CNY or EUR), separately from language | Until you change or clear it |
| ss_vid | localStorage | Random visitor ID for first-party analytics (never shared with third parties) | Until you clear it |
| edu.locale | localStorage (edu site) | Education site language preference | Until you clear it |

Blocking essential cookies may break sign-in, purchasing, downloads, or security verification. You can clear cookies and local storage in your browser at any time.

### 4.2 Cloudflare

Our sites are served through Cloudflare (DNS, CDN, security). Cloudflare may process request data and set cookies strictly needed for security challenges; see Appendix A and Cloudflare's privacy policy.

### 4.3 No third-party tracking

We embed no third-party analytics, advertising pixels, or cross-site tracking. Web fonts are self-hosted at build time — your browser makes no runtime requests to font services. If this ever changes, we will update this Policy and request consent where required.

---

## 5. How We Share Personal Data

- **Service providers:** the infrastructure, communications, payment, and invoicing providers listed in Appendix A. For processing on our behalf, they act under applicable data-protection terms and our instructions. Payment institutions may also act as independent controllers for their own legal obligations, payment security, and fraud prevention, as described in Appendix A and their privacy notices;
- **No selling:** we do not sell or rent personal data, and we do not provide your data or content to third parties for AI model training;
- **Organizations and institutions:** if your account belongs to a team or education institution, its administrators can see membership and seat information (member email, display name, seat status, enrollment validity). Administrators cannot access your Unreal Engine projects or local content through us. Institutions must obtain any consent required from students (and guardians of minors) before enrolling them;
- **Public display:** showcase submissions you confirm for publication are displayed publicly with the attribution you choose; you may ask us to take a published submission down at any time;
- **Business transfers:** in a merger, restructuring, or asset transfer, data may be transferred with the business; we will inform you of the recipient, which remains bound by this Policy;
- **Legal compliance:** we disclose data only within the scope required by valid legal process, after verifying the authority and scope of the request.

---

## 6. International Data Transfers and Storage

### 6.1 Where your data lives

**Our primary application servers and database are hosted in mainland China** (Tencent Cloud, Guangzhou region, Guangdong Province). Supporting services process data in other locations:

- Cloudflare (US-based, global network): CDN and security; its R2 object storage now holds only our daily database and cache backups, which are a full export of the production database and therefore contain the account, licence, order, and security-log data described in this Policy. Installers are distributed from Google Drive and Baidu Netdisk, showcase videos are hosted by Bilibili and YouTube, and neither passes through R2;
- Google (US): sign-in authentication, if you use Google Sign-In;
- Stripe, Inc. and its affiliates (global infrastructure): card payment, customer linkage, refunds, and related invoices. Stripe receives card credentials directly and receives necessary purchaser names, emails, billing countries and addresses, account / organization and order references, amounts, and currencies from us; it returns transaction and invoice information. EUR pricing or a UK receiving account does not mean processing is confined to the UK or Europe. WeChat Pay or offline-transfer orders that do not use Stripe do not by themselves cause those orders' billing data to be sent to Stripe; existing Stripe transactions remain subject to the retention and processing described here;
- Tencent Exmail (China): email delivery for verification codes, notifications, and correspondence;
- Tencent WeCom: internal notifications of successful subscription payments, within the data scope described in Section 2.3 and Appendix A.9.

If you use the services from outside China, your data is transferred to and processed on our servers in China. Support and operations personnel access data from China under access controls.

### 6.2 For users in the EEA, UK, and other regions with transfer rules

If you are in a region whose law restricts international transfers (e.g., GDPR Chapter V), using our services involves transferring your data to China, a jurisdiction without an EU/UK adequacy decision. For such transfers we rely on appropriate safeguards — contractual data-protection commitments modeled on the European Commission's Standard Contractual Clauses (or the UK equivalent) — together with technical measures (encryption in transit, access control, minimization, passwordless design). You may contact us for more information about the safeguards applied to your data.

### 6.3 For users in mainland China

Your account and license data is held on servers within mainland China, and its daily backup copy is stored overseas with Cloudflare (R2). That backup, together with the other limited scenarios above (Cloudflare's global network; optional Google Sign-In; international card payments), is what involves overseas recipients; the Chinese version of this Policy contains the statutory PIPL disclosure, and where required we obtain your separate consent and fulfill applicable procedures.

---

## 7. Data Retention

We keep personal data only as long as needed for the purposes described here, subject to statutory accounting, tax, security, and litigation requirements.

| Category | Retention |
|---|---|
| Account and license records | For the life of the account or license; anonymized after account deletion (Section 8.3), except records we must keep by law |
| Email verification codes | Hash only, for minutes (until verified or expired) |
| Sign-in, session, and network logs | At least the statutory log-retention period (no less than 6 months); records tied to a security incident or dispute may be kept up to 3 years after resolution |
| Device activation and risk records | During the license term and up to 3 years after; the first-trial sign-in record is kept long-term with minimal fields (account, device hash, time) to enforce the one-trial rule |
| Orders, payments, refunds, invoices | Statutory accounting/tax retention periods, then deleted or anonymized |
| Support tickets and correspondence | Up to 3 years after closure; project files you email us are deleted promptly once the issue is resolved |
| Site analytics | Raw logs per the log-retention line above; aggregates contain no direct identifiers |
| Consent records | For the life of the related account/order and statutory evidence periods |
| Backups | Overwritten on backup rotation; isolated if immediate deletion is technically infeasible |

---

## 8. Your Rights and Choices

### 8.1 Rights

Depending on your jurisdiction (GDPR, UK GDPR, PIPL, US state privacy laws, and others), you may have the right to: access and obtain a copy of your data; rectify or complete it; delete it; restrict or object to processing (including objecting to legitimate-interest processing); data portability; withdraw consent at any time (without affecting prior processing); not be subject to solely automated decisions with legal or similarly significant effects; and lodge a complaint with a supervisory authority (for EEA/UK users, your local data protection authority; for users in China, the authorities responsible for personal information protection).

We do not discriminate against you for exercising your rights.

### 8.2 Self-service

In your account center you can directly: edit your profile and language; add or remove secondary email addresses; unlink Google Sign-In; view and revoke sign-in sessions (including "revoke all other sessions"); sign UE devices out remotely; revoke license activations; and request account deletion.

### 8.3 Account deletion

You can request deletion in account settings. A **30-day cooling-off period** applies, during which you may cancel the request. Afterwards the account is anonymized: the email is replaced with a non-identifying placeholder, profile fields are cleared, external identities are unlinked, and sessions are terminated. Records we must keep by law (transactions, tax, security, dispute records) are retained with restricted processing. Deletion is irreversible and forfeits remaining licenses — please settle outstanding matters first.

### 8.4 Exercising other rights

Email **yunsio@yunsio.com** with the subject "Privacy request", stating your registered email, the right you wish to exercise, and the necessary scope. We respond within **15 working days** of verifying your identity (for GDPR requests: within one month, extendable as the law allows). We verify identity proportionately (e.g., a code to your registered email) and never ask for excessive information.

We may be unable to fulfil a request where the law requires us to retain data, where it would harm others' rights or reveal trade secrets, or where the request is manifestly unfounded or excessive; we will explain the reason.

---

## 9. Automated Decisions and Anti-Abuse

We use rules and thresholds to detect abnormal sign-ins, bulk registration, device anomalies, and license abuse, including:

- **One trial account per device:** a second trial-only account appearing on the same device fingerprint triggers a temporary suspension of the related trial accounts (currently 3 days; accounts with paid entitlements are exempt);
- Single-online-device enforcement for the UE plugin (a new sign-in supersedes the previous session);
- IP- and rate-based request limiting.

Suspension notices show the reason and the end date. **You can request human review at yunsio@yunsio.com** — we do not make permanent, unappealable decisions by automation alone. We do not use personal data for discriminatory pricing or third-party ad targeting.

---

## 10. Security

- HTTPS encryption across all services; session cookies are HttpOnly and Secure;
- Passwordless architecture (nothing to phish or leak); verification codes stored only as short-lived hashes;
- Device fingerprints accepted only as hashes — raw hardware identifiers are rejected at the server;
- Plugin tokens stored locally under OS-level encryption (DPAPI);
- Role-based access control, least privilege, and audited administrative operations;
- Rate limiting, anomaly detection, and Cloudflare network protection;
- Backups, vulnerability patching, and security updates.

No internet service is absolutely secure. Please protect your email account (it is your sign-in credential), never share verification codes, and review your active sessions if anything looks unusual.

If a personal data breach occurs, we will take immediate remedial action, notify the competent authorities as required by law, and inform affected users of the nature of the breach, likely consequences, measures taken, and steps you can take — via email, account notice, or website announcement, within the timelines applicable law requires (including, where the GDPR applies, notification to the supervisory authority within 72 hours where feasible).

---

## 11. Children

SuperStage is professional software for stage, lighting, and performance visualization and is not directed at children. We do not knowingly collect personal data from children under 14 (or the higher age your local law sets, e.g., under 16 in parts of the EEA) without verifiable parental consent. Education institutions must obtain guardian consent before enrolling minor students. If you believe a child has provided us personal data without proper consent, contact us and we will verify and delete it.

---

## 12. Changes to This Policy

We may update this Policy for legal, technical, or business reasons. Material changes — new data categories, new sensitive-data processing, new overseas recipients, materially longer retention, or reduced rights — will be notified prominently (email, account notice, in-product notice, or website announcement) with a reasonable advance period, and we will re-obtain consent where the law requires. Continuing to use the services does not by itself replace consent that the law requires to be explicit.

---

## 13. Contact and Complaints

- **Company:** Foshan Yierranran Technology Co., Ltd. (Unified Social Credit Code: 91440606MACXPYQA5R)
- **Registered address:** Nanhai District, Foshan, Guangdong Province, China
- **Privacy contact:** yunsio@yunsio.com
- **Website:** https://yunsio.com

If you are in the EEA or UK, you may also complain to your local supervisory authority. If you are in China, you may complain to the authorities responsible for personal information protection. You may also bring proceedings before a competent court.

This Policy is primarily governed by the laws of the People's Republic of China; mandatory data-protection law applicable to you in your jurisdiction remains unaffected.

---

# Appendix A: Service Providers and International Recipients

> This list is kept consistent with our production environment and updated when providers change.

## A.1 Tencent Cloud (primary hosting)

- **Provider:** Tencent Cloud (Tencent Cloud Computing (Beijing) Co., Ltd.)
- **Privacy policy:** https://privacy.qq.com/
- **Location:** Guangzhou region, Guangdong Province, China
- **Role/purpose:** cloud servers and database hosting all account, license, order, and security data
- **Data:** all server-side personal data described in Section 2

## A.2 Cloudflare (CDN, security, object storage)

- **Provider:** Cloudflare, Inc., 101 Townsend St, San Francisco, CA 94107, USA
- **Privacy policy:** https://www.cloudflare.com/privacypolicy/
- **Role/purpose:** DNS, CDN, security protection; R2 object storage holding the daily off-site backups of our database and cache
- **Data:** IP address, User-Agent, request URLs and timing, security logs, necessary security cookies; the backup files themselves contain all of the server-side personal information described in Section 2
- **Location:** global network; the backup bucket carries an Asia-Pacific location hint
- **Note:** R2 holds nothing but those backups. Installers come from Google Drive and Baidu Netdisk, showcase videos from Bilibili and YouTube, the manual carries no uploaded images, and tickets no longer accept attachments

## A.3 Tencent Exmail (email)

- **Provider:** Tencent (Exmail enterprise email)
- **Privacy policy:** https://privacy.qq.com/
- **Role/purpose:** sending and receiving verification codes, notifications, support and business email (yunsio.com sender domain)
- **Data:** email addresses, message content, attachments, transmission metadata
- **Location:** primarily China

## A.4 Google (optional sign-in)

- **Provider:** Google LLC, 1600 Amphitheatre Parkway, Mountain View, CA 94043, USA
- **Privacy policy:** https://policies.google.com/privacy
- **Role/purpose:** optional Google Sign-In (OAuth; scopes: openid, email, profile)
- **Data:** authentication handled by Google as an independent controller; we receive the account identifier, email, name, avatar URL, and language

## A.5 Stripe (card payments and invoicing)

- **Provider:** Stripe, Inc. and its affiliates
- **Location:** United States and Stripe's global infrastructure
- **Privacy policy:** https://stripe.com/privacy
- **Purpose:** payment-customer management, card payments, refunds, reconciliation, commercial invoices and credit notes, payment fraud prevention
- **Data:** card number, expiry date, and security code (collected directly inside Stripe's component; we do not receive them); purchaser name, billing email, country and address, account / organization and order references, amount and currency supplied by us; customer and transaction identifiers, invoice details and links, payment / refund / invoice status; IP addresses and fraud-prevention device signals processed by Stripe
- **Role:** depending on the activity, a processor following our lawful instructions for transaction data, or an independent controller for its own legal obligations, payment security, and fraud prevention, under its privacy policy and applicable terms
- **Note:** the card-entry component communicates directly with Stripe; our checkout service separately saves and transmits billing details. Processing can occur across Stripe's global infrastructure and is not limited to Europe because an order is priced in EUR

## A.6 WeChat Pay (CNY scan-to-pay)

- **Provider:** Tenpay Payment Technology Co., Ltd. (WeChat Pay)
- **Location:** mainland China
- **Role/purpose:** processing CNY Native (QR) payments, payment result notifications, and refunds
- **Data:** your WeChat Pay account credentials and payment action (handled entirely by WeChat Pay — we never see them), amount and currency, merchant order number, WeChat transaction id
- **Note:** we receive only the payment result notification and transaction id, used to activate the licence and reconcile accounts

## A.7 Banks and remittance channels (offline payment)

- **Provider:** the receiving bank confirmed with you during order communication
- **Role/purpose:** receiving bank transfers, payment verification, refunds
- **Data:** remitter name, amount, currency, transaction reference
- **Note:** applies to enterprise purchases and staff-created orders; for online payment channels see A.5 and A.6

## A.8 Professional advisers and authorities

Lawyers, accountants, auditors, and courts or regulators acting within their legal powers may access the minimum information directly relevant to their function, under statutory or contractual confidentiality.

## A.9 Tencent WeCom (internal payment notifications)

- **Service:** Tencent WeCom bot messaging
- **Purpose:** notifying authorized operations staff of successfully paid subscription orders for fulfillment and reconciliation
- **Data:** username or display name, account email, billing country if provided, plan, term, amount received and currency, a link to the related order, and daily / monthly aggregate receipts without individual customer breakdowns
- **Scope:** internal operational messages, not publication to other customers; messages do not include full billing addresses, card details, license keys, or project content

---

*The Chinese version of this Policy is the authoritative version to the extent permitted by applicable law; this English version is independently drafted for international users, and nothing in it reduces rights you enjoy under mandatory local law.*
