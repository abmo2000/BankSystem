const state = {
    token: "",
    role: "",
    session: null,
};

const statsGrid = document.getElementById("statsGrid");
const loginForm = document.getElementById("loginForm");
const logoutButton = document.getElementById("logoutButton");
const flashMessage = document.getElementById("flashMessage");
const sessionSubtitle = document.getElementById("sessionSubtitle");
const sessionContent = document.getElementById("sessionContent");

function formatCurrency(value) {
    return new Intl.NumberFormat("en-US", {
        style: "currency",
        currency: "EGP",
        maximumFractionDigits: 2,
    }).format(value);
}

async function apiFetch(path, options = {}) {
    const headers = {
        "Content-Type": "application/json",
        ...(options.headers || {}),
    };

    if (state.token) {
        headers["X-Session-Token"] = state.token;
    }

    const response = await fetch(path, { ...options, headers });
    const data = await response.json();
    if (!response.ok) {
        throw new Error(data.message || "Request failed.");
    }
    return data;
}

function showMessage(message, type = "success") {
    flashMessage.textContent = message;
    flashMessage.className = `flash-message ${type}`;
}

function clearMessage() {
    flashMessage.textContent = "";
    flashMessage.className = "flash-message hidden";
}

function renderStats(stats) {
    const items = [
        ["Clients", stats.clients],
        ["Employees", stats.employees],
        ["Admins", stats.admins],
        ["Users", stats.totalUsers],
        ["Client Funds", formatCurrency(stats.clientFunds)],
    ];

    statsGrid.innerHTML = items.map(([label, value]) => `
        <article class="stat-card">
            <span>${label}</span>
            <strong>${value}</strong>
        </article>
    `).join("");
}

function renderRows(rows, valueMapper) {
    return rows.map((row) => `
        <tr>
            ${valueMapper(row).map((value) => `<td>${value}</td>`).join("")}
        </tr>
    `).join("");
}

function bindForm(selector, handler) {
    const form = sessionContent.querySelector(selector);
    if (!form) {
        return;
    }

    form.addEventListener("submit", async (event) => {
        event.preventDefault();
        clearMessage();
        const payload = Object.fromEntries(new FormData(form).entries());

        try {
            await handler(payload);
            form.reset();
            await refreshSession();
        } catch (error) {
            showMessage(error.message, "error");
        }
    });
}

function renderClient(session) {
    const template = document.getElementById("clientTemplate").content.cloneNode(true);
    template.querySelector('[data-field="clientName"]').textContent = `${session.user.name} (#${session.user.id})`;
    template.querySelector('[data-field="clientBalance"]').textContent = formatCurrency(session.user.balance);
    sessionContent.innerHTML = "";
    sessionContent.className = "session-content";
    sessionContent.appendChild(template);

    bindForm('[data-form="deposit"]', async (payload) => {
        const data = await apiFetch("/api/client/deposit", {
            method: "POST",
            body: JSON.stringify({ clientId: String(session.user.id), amount: payload.amount }),
        });
        showMessage(data.message);
    });

    bindForm('[data-form="withdraw"]', async (payload) => {
        const data = await apiFetch("/api/client/withdraw", {
            method: "POST",
            body: JSON.stringify({ clientId: String(session.user.id), amount: payload.amount }),
        });
        showMessage(data.message);
    });

    bindForm('[data-form="transfer"]', async (payload) => {
        const data = await apiFetch("/api/client/transfer", {
            method: "POST",
            body: JSON.stringify({
                senderId: String(session.user.id),
                recipientId: payload.recipientId,
                amount: payload.amount,
            }),
        });
        showMessage(data.message);
    });
}

function renderEmployee(session) {
    const template = document.getElementById("employeeTemplate").content.cloneNode(true);
    template.querySelector('[data-field="employeeName"]').textContent = `${session.user.name} (#${session.user.id})`;
    template.querySelector('[data-field="employeeSalary"]').textContent = formatCurrency(session.user.salary);
    template.querySelector('[data-field="clientCount"]').textContent = session.clients.length;
    template.querySelector('[data-field="clientsTable"]').innerHTML = renderRows(session.clients, (client) => [client.id, client.name, formatCurrency(client.balance)]);
    sessionContent.innerHTML = "";
    sessionContent.className = "session-content";
    sessionContent.appendChild(template);

    bindForm('[data-form="createClient"]', async (payload) => {
        const data = await apiFetch("/api/employee/client/create", {
            method: "POST",
            body: JSON.stringify(payload),
        });
        showMessage(data.message);
    });

    bindForm('[data-form="updateClient"]', async (payload) => {
        const data = await apiFetch("/api/employee/client/update", {
            method: "POST",
            body: JSON.stringify(payload),
        });
        showMessage(data.message);
    });
}

function renderAdmin(session) {
    const template = document.getElementById("adminTemplate").content.cloneNode(true);
    template.querySelector('[data-field="adminName"]').textContent = `${session.user.name} (#${session.user.id})`;
    template.querySelector('[data-field="adminSalary"]').textContent = formatCurrency(session.user.salary);
    template.querySelector('[data-field="employeeCount"]').textContent = session.employees.length;
    template.querySelector('[data-field="employeesTable"]').innerHTML = renderRows(session.employees, (employee) => [employee.id, employee.name, formatCurrency(employee.salary)]);
    template.querySelector('[data-field="adminsTable"]').innerHTML = renderRows(session.admins, (admin) => [admin.id, admin.name, formatCurrency(admin.salary)]);
    sessionContent.innerHTML = "";
    sessionContent.className = "session-content";
    sessionContent.appendChild(template);

    bindForm('[data-form="createEmployee"]', async (payload) => {
        const data = await apiFetch("/api/admin/employee/create", {
            method: "POST",
            body: JSON.stringify(payload),
        });
        showMessage(data.message);
    });

    bindForm('[data-form="updateEmployee"]', async (payload) => {
        const data = await apiFetch("/api/admin/employee/update", {
            method: "POST",
            body: JSON.stringify(payload),
        });
        showMessage(data.message);
    });
}

async function refreshDashboard() {
    const data = await apiFetch("/api/dashboard", { method: "GET" });
    renderStats(data.stats);
}

async function refreshSession() {
    if (!state.token) {
        state.session = null;
        sessionSubtitle.textContent = "No active user session.";
        sessionContent.textContent = "Log in to access banking tools.";
        sessionContent.className = "session-content empty-state";
        return;
    }

    try {
        const session = await apiFetch("/api/session", { method: "GET" });
        state.session = session;
        state.role = session.role;
        sessionSubtitle.textContent = `Active role: ${session.role}`;

        if (session.role === "client") {
            renderClient(session);
        } else if (session.role === "employee") {
            renderEmployee(session);
        } else {
            renderAdmin(session);
        }
    } catch (error) {
        state.token = "";
        state.session = null;
        state.role = "";
        sessionSubtitle.textContent = "No active user session.";
        sessionContent.textContent = "Log in to access banking tools.";
        sessionContent.className = "session-content empty-state";
        showMessage(error.message, "error");
    }
}

loginForm.addEventListener("submit", async (event) => {
    event.preventDefault();
    clearMessage();
    const payload = Object.fromEntries(new FormData(loginForm).entries());

    try {
        const data = await apiFetch("/api/login", {
            method: "POST",
            body: JSON.stringify(payload),
        });
        state.token = data.token;
        state.role = data.role;
        showMessage(data.message);
        await refreshSession();
        await refreshDashboard();
    } catch (error) {
        showMessage(error.message, "error");
    }
});

logoutButton.addEventListener("click", async () => {
    if (!state.token) {
        showMessage("No active session to close.", "error");
        return;
    }

    try {
        const data = await apiFetch("/api/logout", { method: "POST", body: "{}" });
        state.token = "";
        state.role = "";
        state.session = null;
        showMessage(data.message);
        await refreshSession();
        await refreshDashboard();
    } catch (error) {
        showMessage(error.message, "error");
    }
});

async function init() {
    await refreshDashboard();
    await refreshSession();
}

init().catch((error) => {
    showMessage(error.message, "error");
});