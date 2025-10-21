// LifeOS - Static Version
// Simple JSON loading and display with filtering

class LifeOS {
    constructor() {
        this.programs = [];
        this.bookmarks = [];
        this.goals = [];
        this.filteredPrograms = [];
        
        this.init();
    }

    async init() {
        await this.loadData();
        this.setupEventListeners();
        this.renderAll();
    }

    async loadData() {
        try {
            // Load all JSON files
            const [programsResponse, bookmarksResponse, goalsResponse] = await Promise.all([
                fetch('programs.json'),
                fetch('bookmarks.json'),
                fetch('goals.json')
            ]);

            this.programs = await programsResponse.json();
            this.bookmarks = await bookmarksResponse.json();
            this.goals = await goalsResponse.json();

            this.filteredPrograms = [...this.programs];
        } catch (error) {
            console.error('Error loading data:', error);
            this.showError('Failed to load data. Please check that all JSON files exist.');
        }
    }

    setupEventListeners() {
        // Tab switching
        document.querySelectorAll('.tab-button').forEach(button => {
            button.addEventListener('click', (e) => {
                this.switchTab(e.target.dataset.tab);
            });
        });

        // Program filtering
        const searchInput = document.getElementById('searchInput');
        const cityFilter = document.getElementById('cityFilter');
        const typeFilter = document.getElementById('typeFilter');

        searchInput.addEventListener('input', () => this.filterPrograms());
        cityFilter.addEventListener('change', () => this.filterPrograms());
        typeFilter.addEventListener('change', () => this.filterPrograms());
    }

    switchTab(tabName) {
        // Update tab buttons
        document.querySelectorAll('.tab-button').forEach(btn => {
            btn.classList.remove('active');
        });
        document.querySelector(`[data-tab="${tabName}"]`).classList.add('active');

        // Update tab content
        document.querySelectorAll('.tab-content').forEach(content => {
            content.classList.remove('active');
        });
        document.getElementById(tabName).classList.add('active');
    }

    filterPrograms() {
        const searchTerm = document.getElementById('searchInput').value.toLowerCase();
        const cityFilter = document.getElementById('cityFilter').value;
        const typeFilter = document.getElementById('typeFilter').value;

        this.filteredPrograms = this.programs.filter(program => {
            const matchesSearch = !searchTerm || 
                program.title.toLowerCase().includes(searchTerm) ||
                program.description.toLowerCase().includes(searchTerm);
            
            const matchesCity = !cityFilter || program.city === cityFilter;
            const matchesType = !typeFilter || program.type === typeFilter;

            return matchesSearch && matchesCity && matchesType;
        });

        this.renderPrograms();
    }

    renderAll() {
        this.renderPrograms();
        this.renderBookmarks();
        this.renderGoals();
        this.populateFilters();
    }

    renderPrograms() {
        const container = document.getElementById('programsList');
        
        if (this.filteredPrograms.length === 0) {
            container.innerHTML = '<div class="empty-state"><h3>No programs found</h3><p>Try adjusting your filters</p></div>';
            return;
        }

        container.innerHTML = this.filteredPrograms.map(program => `
            <div class="program-card">
                <h3 class="program-title">${this.escapeHtml(program.title)}</h3>
                <p class="program-description">${this.escapeHtml(program.description)}</p>
                <div class="program-meta">
                    <span class="meta-tag">${this.escapeHtml(program.city)}</span>
                    <span class="meta-tag">${this.escapeHtml(program.type)}</span>
                    ${program.duration ? `<span class="meta-tag">${this.escapeHtml(program.duration)}</span>` : ''}
                </div>
                ${program.url ? `<a href="${program.url}" target="_blank" class="program-link">Visit Program</a>` : ''}
            </div>
        `).join('');
    }

    renderBookmarks() {
        const container = document.getElementById('bookmarksList');
        
        if (this.bookmarks.length === 0) {
            container.innerHTML = '<div class="empty-state"><h3>No bookmarks yet</h3><p>Add some bookmarks to your bookmarks.json file</p></div>';
            return;
        }

        container.innerHTML = this.bookmarks.map(bookmark => `
            <div class="bookmark-item">
                <h3 class="bookmark-title">${this.escapeHtml(bookmark.title)}</h3>
                <a href="${bookmark.url}" target="_blank" class="bookmark-url">${this.escapeHtml(bookmark.url)}</a>
                ${bookmark.description ? `<p class="bookmark-description">${this.escapeHtml(bookmark.description)}</p>` : ''}
            </div>
        `).join('');
    }

    renderGoals() {
        const container = document.getElementById('goalsList');
        
        if (this.goals.length === 0) {
            container.innerHTML = '<div class="empty-state"><h3>No goals yet</h3><p>Add some goals to your goals.json file</p></div>';
            return;
        }

        container.innerHTML = this.goals.map(goal => `
            <div class="goal-item">
                <h3 class="goal-title">${this.escapeHtml(goal.title)}</h3>
                <p class="goal-description">${this.escapeHtml(goal.description)}</p>
                <div class="goal-meta">
                    ${goal.category ? `<span class="goal-tag goal-category">${this.escapeHtml(goal.category)}</span>` : ''}
                    ${goal.priority ? `<span class="goal-tag goal-priority">${this.escapeHtml(goal.priority)}</span>` : ''}
                    ${goal.status ? `<span class="goal-tag">${this.escapeHtml(goal.status)}</span>` : ''}
                </div>
            </div>
        `).join('');
    }

    populateFilters() {
        // Populate city filter
        const cities = [...new Set(this.programs.map(p => p.city))].sort();
        const cityFilter = document.getElementById('cityFilter');
        cityFilter.innerHTML = '<option value="">All Cities</option>' + 
            cities.map(city => `<option value="${city}">${this.escapeHtml(city)}</option>`).join('');

        // Populate type filter
        const types = [...new Set(this.programs.map(p => p.type))].sort();
        const typeFilter = document.getElementById('typeFilter');
        typeFilter.innerHTML = '<option value="">All Types</option>' + 
            types.map(type => `<option value="${type}">${this.escapeHtml(type)}</option>`).join('');
    }

    escapeHtml(text) {
        const div = document.createElement('div');
        div.textContent = text;
        return div.innerHTML;
    }

    showError(message) {
        const container = document.querySelector('.container');
        container.innerHTML = `
            <div class="error-state">
                <h2>Error</h2>
                <p>${message}</p>
                <button onclick="location.reload()">Reload Page</button>
            </div>
        `;
    }
}

// Initialize the app when the page loads
document.addEventListener('DOMContentLoaded', () => {
    new LifeOS();
});
