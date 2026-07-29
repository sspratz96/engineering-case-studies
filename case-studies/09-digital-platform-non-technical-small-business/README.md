# Digital Platform for a Non-Technical Small Business

![Status](https://img.shields.io/badge/status-Internal%20Development-yellow)
![Focus](https://img.shields.io/badge/focus-Product%20Engineering-success)
![Architecture](https://img.shields.io/badge/architecture-Serverless%20%7C%20Low--Cost-orange)
![Stack](https://img.shields.io/badge/stack-Firebase%20%7C%20Apps%20Script%20%7C%20Google%20Sheets-lightgrey)

## Executive Summary

This case study documents the design and implementation of a lightweight digital platform for a small business led by non-technical founders.

The business combined two related initiatives:

- **Bichos Club**, a product and ecommerce concept initially aimed at schools and families
- **Taller de Bichos**, an educational workshop offering that required event discovery, booking requests and capacity management

The founders had a clear vision of what they wanted customers to see, but they did not have the technical background required to translate that vision into application flows, data structures, security boundaries or operational tools.

My role was to act as the technical translator between the business idea and the system needed to support it.

The resulting platform brought together:

- public brand and product experiences
- workshop discovery and reservations
- capacity and booking management
- editable website content
- a lightweight CRM
- email automation
- authenticated administrative workflows
- separate development and public access modes
- automated deployment checks

The main engineering challenge was not building an isolated website.

It was turning loosely defined business requirements into a coherent, operable and maintainable platform that could be used by people without coding knowledge and without introducing unnecessary infrastructure or cost.

---

# Context

Bichos Club began as a small entrepreneurial initiative created by two founders without software development experience.

They understood the brand, audience and visual experience they wanted to create, but their requirements initially existed as ideas, examples and desired screens rather than as a formal product specification.

At the same time, Taller de Bichos had a related but distinct purpose. It needed to communicate educational activities and support workshop reservations, participant management and follow-up.

The challenge was to allow both initiatives to coexist within one digital ecosystem while preserving their different user journeys.

The platform also needed to remain practical for a small team during internal development and after launch:

- low initial cost
- minimal infrastructure maintenance
- content editable without code
- simple operational workflows
- protection of personal and commercial information
- a clear path toward future ecommerce growth

---

# Current Project Status

At the time of writing, the platform remains under internal development and has not yet been launched publicly.

The source code is maintained in a private repository, with access currently limited to the two founders and myself. This controlled environment allows us to validate the product experience, administrative workflows, reservation logic, data access boundaries and deployment configuration before making the platform available to external users.

A first public launch is tentatively planned for **November 2026**.

The capabilities and outcomes described in this case study therefore refer to the current internal implementation and its validated workflows. They should not be interpreted as evidence of active public usage, customer adoption or production-scale performance.

---

# The Problem

A conventional marketing website would not have been enough.

The business needed to support multiple types of users and workflows:

- visitors discovering the brand
- customers exploring products
- families requesting workshop reservations
- administrators updating workshops and content
- founders following commercial opportunities
- authorised users reviewing operational information

The initial requirements were also ambiguous. The founders could explain what they wanted to see, but several technical questions still had to be resolved:

- Which information should be public?
- Which information contained personal data?
- How should workshop capacity be represented?
- What should happen when a reservation was cancelled or reactivated?
- How could the founders update the website without editing source code?
- How should ecommerce coexist with workshop operations?
- Which components needed authentication?
- What architecture was proportional to the expected traffic and budget?
- Which parts should remain custom and which should eventually move to Shopify?

The problem was therefore both technical and organisational.

The business needed a usable platform, but first its informal requirements had to be converted into explicit workflows, entities, permissions and operational rules.

---

# My Role

I worked across product discovery, architecture, implementation and deployment.

My responsibilities included:

- translating non-technical ideas into product requirements
- identifying user journeys and administrative workflows
- defining the separation between public and private information
- designing the application and data architecture
- implementing public pages and internal interfaces
- building workshop reservation logic
- implementing content administration workflows
- integrating Firebase, Google Apps Script and Google Sheets
- designing a lightweight CRM
- adding authentication and backend validation
- automating email and deployment processes
- documenting technical constraints and future migration paths

This project required more than writing code.

It required questioning incomplete requirements, identifying hidden operational needs and designing a system that the founders could realistically use.

---

# Product Scope

The platform evolved into a small digital operating system for the business.

## Public Experience

The public-facing platform included distinct but connected experiences for:

- Bichos Club
- product exploration
- Taller de Bichos
- workshop information
- Cristobal's public profile and educational content
- business-to-business enquiries
- contact and lead capture

This allowed different activities to coexist without requiring completely separate platforms.

## Workshop Operations

The workshop workflow supported:

- available workshops and dates
- total and available capacity
- reservation requests
- participant quantities
- duplicate detection
- booking status changes
- cancellations
- reactivations
- capacity release and consumption
- confirmation communication
- operational history

## Administrative Operations

Authorised users could manage information such as:

- workshops
- schedules
- products
- website content
- stories and countries
- gallery content
- bookings
- business opportunities

The objective was to reduce dependency on source-code changes for routine business operations.

## Lightweight CRM

A separate CRM interface supported basic commercial follow-up, including:

- contacts
- opportunities
- actions
- internal notes
- selected public interaction signals

The CRM was intentionally lightweight and aligned with the scale of the business.

---

# Architecture

The architecture prioritised simplicity, low operational cost and tools familiar to the founders.

```text
Firebase Hosting
│
├── Public brand website
├── Product and ecommerce experience
├── Taller de Bichos landing pages
├── Administrative interface
└── CRM interface
         │
         ▼
Google Apps Script
│
├── Public content endpoints
├── Reservation operations
├── Authentication validation
├── Email automation
├── CRM operations
└── Business rules
         │
         ▼
Google Sheets
├── Products and public content
├── Workshops and schedules
├── Reservations
├── Audit records
└── CRM data
```

This architecture avoided introducing a traditional application server or a managed relational database before the business needed them.

It also allowed structured information to remain accessible through tools already familiar to the team.

---

# Why This Architecture

## Low Operational Overhead

A small business without a technical team should not inherit infrastructure that requires constant maintenance.

Firebase Hosting provided simple static deployment, while Apps Script supplied backend logic without the need to operate a dedicated server.

## Familiar Administration

Google Sheets provided an accessible operational surface for structured business information.

The founders could understand and inspect the underlying data without specialised database tools.

## Fast Validation

The lightweight stack allowed product ideas and workflows to be validated before committing to a larger ecommerce or backend architecture.

## Controlled Evolution

The architecture was not treated as the permanent solution for every future scale.

It was a proportional first system that could support early operations while preserving a path toward Shopify and stronger backend services when justified by business growth.

---

# Core Workflow: Workshop Reservation

The reservation process connected the public experience with operational rules and persistent records.

```text
Visitor selects a workshop
        ↓
Workshop availability is retrieved
        ↓
Request fields are validated
        ↓
Duplicate conditions are checked
        ↓
Capacity is updated under a lock
        ↓
Reservation record is stored
        ↓
Confirmation communication is triggered
```

The workflow needed to protect capacity consistency.

A booking was not simply appended to a spreadsheet. The system checked the relevant workshop, participant quantity and current state before updating availability.

---

# Reservation State Management

Bookings could change over time.

The system therefore supported explicit state transitions rather than destructive deletion.

When a reservation was cancelled, the backend:

1. verified that the record had not been modified by another operation
2. identified the associated workshop and participant count
3. returned capacity to the workshop
4. changed the reservation status
5. recorded who performed the action and why
6. stored an audit event

A second cancellation did not release capacity again.

This helped prevent common operational inconsistencies such as duplicated capacity restoration or stale administrative updates.

---

# Concurrency and Consistency

Google Sheets is not a transactional database, but the booking workflow still required transaction-like behaviour.

The implementation used locking and revision checks to reduce the risk of:

- two users consuming the same remaining capacity
- a cancellation releasing capacity twice
- an administrator overwriting a more recent update
- reservation state becoming inconsistent with workshop availability

The architecture remained lightweight, but the business rules could not be treated casually simply because the data lived in a spreadsheet.

---

# Public and Private Data Boundaries

The platform contained both public content and private operational data.

These categories were intentionally separated.

Public pages could retrieve only approved datasets and fields.

Private information such as participant details, family information, internal follow-up and CRM records was excluded from public endpoints.

The backend used allowlists to control which sheets and fields could be exposed.

The browser was considered an untrusted environment. Sensitive access decisions were validated by backend logic.

---

# Authentication and Defensive Controls

Administrative and CRM workflows required authenticated access.

The system separated public content, testing modes, internal administrative workflows and private CRM operations.

Authentication tokens were validated before protected operations were executed.

The backend also applied controls such as:

- allowed field definitions
- type validation
- length limits
- identifier validation
- formula neutralisation
- controlled status values
- revision checks
- explicit public-field selection

These controls reduced the risk of accidental corruption, unauthorised data exposure and formula injection.

---

# Content Management

One of the central product requirements was that routine updates should not require a developer.

The administrative interface allowed authorised users to manage selected operational and public information without directly editing the application source.

This included workshops, schedules, products, stories, gallery records and reservation statuses.

The objective was not to build a generic content management system. It was to give the founders control over the specific information they needed to operate the business.

---

# Deployment and Environment Controls

The project used GitHub Actions and Firebase Hosting to automate deployment tasks.

The deployment workflow included checks for:

- access mode configuration
- security-sensitive settings
- unresolved configuration placeholders
- expected hosting behaviour
- selected performance constraints

A deployment could fail when critical assumptions were not satisfied.

The project also distinguished between development and public environments so that internal functionality could be tested without treating every build as a production release.

---

# Key Engineering Decisions

## 1. Translate the Business Before Selecting Technology

The founders' initial ideas were expressed visually and operationally rather than technically.

The first engineering task was therefore to identify users, workflows, entities, states and constraints.

## 2. Use Architecture Proportional to the Business

A custom server, managed database and large frontend framework would have increased cost and maintenance before the business had validated the workflows.

The selected architecture was deliberately lightweight.

## 3. Make Operations Accessible to Non-Technical Users

The platform was designed so that routine content and booking management did not require source-code changes.

## 4. Keep Security Boundaries in the Backend

Client-side visibility controls were not treated as sufficient protection.

Sensitive fields and operations were restricted through backend validation and explicit allowlists.

## 5. Preserve History Instead of Deleting Records

Bookings and commercial records were archived or moved through explicit states rather than removed permanently.

## 6. Design for Migration, Not Premature Scale

The custom platform supported validation and early operations.

The ecommerce component retained a future path toward Shopify rather than attempting to recreate every mature commerce capability internally.

---

# Trade-Offs

## Google Sheets Instead of a Database

Google Sheets improved accessibility and reduced setup cost, but introduced limitations in concurrency, schema enforcement, relational modelling, observability, migration management and large-scale performance.

It was appropriate for the early stage and expected volume, but it should not be treated as the optimal storage layer at every scale.

## Static Pages Instead of a Component Framework

The static architecture accelerated implementation and reduced build complexity.

As the number of pages and workflows increased, large HTML and JavaScript files became harder to maintain.

A future version would extract reusable components and separate domain logic more formally.

## Custom Ecommerce Experience Instead of Immediate Shopify Adoption

A custom experience made rapid visual and workflow validation possible.

A production ecommerce operation would benefit from Shopify's mature payment, inventory, order and fulfilment capabilities.

## Lightweight CRM Instead of a Commercial Product

The custom CRM matched the initial workflow and avoided unnecessary licensing.

As sales volume and collaboration requirements grow, a dedicated CRM may provide stronger automation, reporting and integration capabilities.

---

# Results

The project has converted an informal business vision into an integrated internal platform that is currently being reviewed and refined ahead of its planned public launch.

The internal version currently supports:

- the coexistence of Bichos Club and Taller de Bichos within one digital ecosystem
- structured workshop reservation workflows
- consistent capacity and booking-state management
- content administration without routine source-code changes
- separation between public content and private operational data
- lightweight commercial follow-up through a custom CRM
- automated email and deployment processes
- separate development and public-access configurations
- a practical future path toward Shopify adoption

The platform has also given the founders a concrete environment in which to review the product and refine their requirements.

Instead of discussing only abstract ideas, the three-person team can interact with the workflows, identify missing requirements and make better product decisions before the public release.

Because the platform is not yet publicly available, this case study does not claim active customer adoption, production usage metrics or commercial results.

---

# Lessons Learned

## 1. Non-Technical Stakeholders Often Describe Screens, Not Systems

A requested page usually implies hidden data, state and operational requirements.

The engineering role is to uncover those requirements before implementation.

## 2. Simplicity Is a Business Requirement

For a small team, technical simplicity directly affects whether a system can be operated and sustained.

The best architecture is not automatically the most sophisticated one.

## 3. Spreadsheets Still Require Strong Engineering Controls

Using Sheets does not remove the need for validation, concurrency management, auditability or permission boundaries.

## 4. Administrative Experience Is Part of the Product

A platform is not truly usable if every operational change requires a developer.

## 5. Early Platforms Should Create Learning

The first version provided more than functionality.

It created a shared environment where the founders could test assumptions and clarify what the business actually needed.

---

# What I Would Do Differently Today

## Modularise the Frontend Earlier

As the platform expanded, several pages accumulated substantial HTML, CSS and JavaScript.

I would now establish reusable components, shared modules and clearer domain boundaries earlier in the project.

## Add Automated Tests Around Business Rules

Reservation transitions, capacity changes and access rules should be covered by repeatable automated tests.

## Introduce Structured Observability

A stronger implementation would centralise application errors, failed operations, latency and deployment events.

## Formalise the Data Model

The Sheets-based model could be documented with explicit schemas, identifiers, relationships and migration rules.

## Define the Shopify Boundary Earlier

The platform would benefit from a more explicit decision about which commerce responsibilities remain custom and which are delegated to Shopify.

---

# Future Architecture

```text
Public website and Shopify
          │
          ├── Products and checkout
          ├── Workshop discovery
          └── Customer experience
                    │
                    ▼
          Managed application API
                    │
          ┌─────────┼─────────┐
          ▼         ▼         ▼
    Transactional  CRM     Email service
       database
          │
          ▼
    Reporting and analytics
```

The migration should be triggered by business needs such as transaction volume, concurrency, integration requirements or operational complexity rather than by technology preference alone.

---

# Final Takeaway

The most important outcome of this project was not the creation of another website.

It was the transformation of an ambiguous, non-technical business vision into a functioning digital system with explicit workflows, data boundaries, operational controls and a realistic evolution path.

> Strong engineering begins by making the problem clear enough that the system can be designed deliberately.

The technology mattered, but the most valuable contribution was translating what the founders imagined into something they could operate, test and improve.
